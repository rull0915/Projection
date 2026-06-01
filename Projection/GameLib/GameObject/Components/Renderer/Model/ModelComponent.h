//====================================================//
// ファイル名   : ModelComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/03
//
// 概要 :
//
// 更新履歴 :
// 2026/05/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../RendererComponent.h"
#include "GameLib/Resources/ResourceManager.h"
#include <Model.h>

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ModelComponent : public RendererComponent<ModelComponent, ComponentID::ModelRenderer>
{
    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    DirectX::Model* m_model;

    bool m_useColor;
    int m_color;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ModelComponent(IComponentOwner* own)
        : RendererComponent(own)
        , m_model{ nullptr }
        , m_color{ 0xFFFFFF }
        , m_useColor{ false }
    {
    };
    ~ModelComponent() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 描画関数
    void Draw(Renderer& renderer) override;

    // モデルをセットする関数
    void SetModel(const std::string& keyName)
    {
        m_model = ResourceManager::Instance().GetModel(keyName);
    }
    void SetColor(int color) { m_color = color; }
    void SetUseColor(bool flag) { m_useColor = flag; }
};