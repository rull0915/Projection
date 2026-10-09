//====================================================//
// ファイル名  : MeshRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Renderer/MeshRenderer.h"

#include "Renderer/Renderer.h"
#include "Components/World/Mesh/MeshFilter.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void MeshRenderer::Draw(Renderer& renderer)
	{
		// AssetManagerがなければスキップ
		if (!m_assetManager) return;

		// 同じオブジェクトに付いているMeshFilterを取得
		auto* meshFilter = GetComponent<MeshFilter>();

		// メッシュが設定されていなければ描画しない
		if (!meshFilter || meshFilter->GetMesh() == ERROR_HANDLE<Mesh>) return;

		// トランスフォームからworld行列を取得
		const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

		// Meshを取得
		Mesh* pMesh = m_assetManager->Get(meshFilter->GetMesh());

		// 取得できなければスキップ
		if (!pMesh) return;

		auto& subMeshes = pMesh->GetSubMeshes();

		// SubMeshの数分ループ
		for (size_t i = 0; i < subMeshes.size(); ++i)
		{
			// マテリアルを設定
			renderer.SetMaterial(i < m_materials.size() ? m_materials[i] : ERROR_HANDLE<MaterialAsset>);

			// 行列を使用しモデルを描画
			renderer.Draw().Mesh().DrawMesh(pMesh, i, world);
		}

		// マテリアルのリセット
		renderer.SetMaterial(ERROR_HANDLE<MaterialAsset>);
	}
}	// namespace REngine
