/**
 * @file    MeshRenderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/28
 */

#include "pch.h"
#include "Components/World/Renderer/MeshRenderer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shadow/ShadowRenderer.h"
#include "Components/World/Mesh/MeshFilter.h"
#include "Assets/Managers/AssetManager.h"

namespace RHEngine
{
	MeshRenderer::MeshRenderer(IComponentOwner* pOwner)
		: RendererBase(pOwner)
		, m_materials{}
		, m_pAssetManager{ nullptr }
	{
		ADD_PROPERTY(MeshRenderer, m_materials);
	}

	void MeshRenderer::Draw(Renderer& renderer)
	{
		// AssetManagerがなければスキップ
		if (!m_pAssetManager) return;

		// 同じオブジェクトに付いているMeshFilterを取得
		auto* pMeshFilter = GetComponent<MeshFilter>();

		// メッシュが設定されていなければ描画しない
		if (!pMeshFilter || pMeshFilter->GetMesh() == ERROR_HANDLE<Mesh>) return;

		// トランスフォームからworld行列を取得
		const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

		// Meshを取得
		Mesh* pMesh = m_pAssetManager->Get(pMeshFilter->GetMesh());

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

	void MeshRenderer::DrawToShadowMap(ShadowRenderer& shadowRenderer)
	{
		// AssetManagerがなければスキップ
		if (!m_pAssetManager) return;

		// 同じオブジェクトに付いているMeshFilterを取得
		auto* pMeshFilter = GetComponent<MeshFilter>();

		// メッシュが設定されていなければ描画しない
		if (!pMeshFilter || pMeshFilter->GetMesh() == ERROR_HANDLE<Mesh>) return;

		// トランスフォームからworld行列を取得
		const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

		// Meshを取得
		if (Mesh* pMesh = m_pAssetManager->Get(pMeshFilter->GetMesh()))
		{
			// SubMeshの数分ループ
			for (size_t i = 0; i < pMesh->GetSubMeshes().size(); ++i)
			{
				// 行列を使用しモデルを描画
				shadowRenderer.DrawMesh(pMesh, i, world);
			}
		}
	}
}	// namespace RHEngine

