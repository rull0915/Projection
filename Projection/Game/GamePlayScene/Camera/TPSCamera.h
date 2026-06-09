//====================================================//
// ファイル名   : TPSCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/07
//
// 概要 : カメラをTPS用に動かすコンポーネント
//
// 更新履歴 :
// 2026/06/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class TPSCamera : public Component<TPSCamera, ComponentID::TPSCamera>
{
    // 感度
    static constexpr float SENSITIVITY = 0.5f;

    static constexpr float DISTANCE = 5.0f;

private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    Transform* m_pOwnTransform;
    Transform* m_pTargetTransform;

    DirectX::SimpleMath::Vector2 m_angle;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    TPSCamera(IComponentOwner* owner)
        : Component(owner)
        , m_pOwnTransform{ nullptr }
        , m_pTargetTransform{ nullptr }
        , m_angle{ 0.0f, 0.0f }
    {
    }

    ~TPSCamera() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void LateUpdate(const GameTimer& gameTimer) override;

    void SetTarget(Transform* t) { m_pTargetTransform = t; }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};
