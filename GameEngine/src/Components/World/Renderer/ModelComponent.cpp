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

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void ModelComponent::Start()
	{
		LoadResource();
		ReflectLoading();
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		// モデルがnullなら描画しない
		if (!m_model) return;

		// トランスフォームからworld行列を取得
		const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

		// 行列を使用しモデルを描画
		renderer.Draw().Model().DrawModel(m_model, world);
	}
}	// namespace REngine
