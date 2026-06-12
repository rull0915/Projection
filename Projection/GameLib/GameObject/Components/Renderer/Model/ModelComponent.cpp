
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
#include "ModelComponent.h"

#include "GameLib/Common/Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ModelComponent::Draw(Renderer& renderer)
{
    // モデルがnullなら描画しない
    if (!m_model) return;

    // 透明度を設定する
    renderer.SetAlpha(GetAlpha());

    // トランスフォームからworld行列を取得
    const DirectX::SimpleMath::Matrix& world = GetTransform()->GetWorldMatrix();

    // 行列を使用しモデルを描画
    renderer.Draw().Model(m_model, world);

    // 透明度をリセット
    renderer.SetAlpha(1.0f);
}

void ModelComponent::Save(json& js)
{
    js =
    {
        { "ModelName", m_modelName },
    };
}

void ModelComponent::Load(json & js)
{
    SetModel(js["ModelName"]);
}
