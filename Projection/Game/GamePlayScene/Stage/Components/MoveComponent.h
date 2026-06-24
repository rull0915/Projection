//====================================================//
// ファイル名   : MoveComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 移動コンポーネント 
//          注: RigidBodyとの併用は控えるように
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include <functional>

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class MoveComponent : public Component<MoveComponent, ComponentID::MoveComponent>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------
    static constexpr float BORDER = 1.0f / 1.14159265f;

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 移動の式
    // 引数: 時間
    // 戻り値: 初期位置からの相対座標
    std::function<DirectX::SimpleMath::Vector3(float)> m_moveFunc;

    // 初期位置
    DirectX::SimpleMath::Vector3 m_initPoint;

    // 時間
    float m_sumTime;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    MoveComponent(IComponentOwner* owner)
        : Component(owner)
        , m_moveFunc{}
        , m_initPoint{}
        , m_sumTime{ 0 }
    {
    }

    ~MoveComponent() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void Update(const GameTimer& gameTimer) override;

    void SetFunc(const std::function<DirectX::SimpleMath::Vector3(float)>& func) { m_moveFunc = func; }
private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------
    void OnCollisionEnter(HitContact& contact) override;
    void OnCollisionExit(HitContact& contact) override;
};
