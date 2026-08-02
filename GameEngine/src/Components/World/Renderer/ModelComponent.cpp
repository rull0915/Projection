//====================================================//
// ファイル名  : ModelComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Renderer/ModelComponent.h"

#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void ModelComponent::Start()
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		// AssetManagerがなければ何もしない
		if (!m_assetManager) return;

		// モデルアセットを取得
		Model* model = m_assetManager->Get<Model>(m_modelHandle);

		// モデルがnullなら描画しない
		if (!model) return;

		// トランスフォームからworld行列を取得
		const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

		// 行列を使用しモデルを描画
		renderer.Draw().Model().DrawModel(model->Get(), world);
	}
}	// namespace REngine
