//====================================================//
// ファイル名   : Player.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要 : プレイヤークラス
//
// 更新履歴 :
// 2026/05/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

#include "GameLib/GameObject/Components/Collider/PhysicsMaterial.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Player : public Component<Player, ComponentID::Player>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------
    static constexpr float MOVE_SPEED = 5.0f;   // 移動の速度
    static constexpr float JUMP_POWER = 15.0f;  // ジャンプの力
    static constexpr float CAN_JUMP_BORDER = 1.0f / 1.414f; // ジャンプ可能な地面の傾きのボーダー(cos45°の近似値をボーダーに)

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    Transform* m_pTransform;

    PhysicsMaterial m_material;

    // 現在の次元が2次元かどうか
    bool m_is2D;

    // ジャンプ可能フラグ
    bool m_canJump;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------

    Player(IComponentOwner* owner);
    ~Player();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Start() override;
    void Update(const GameTimer& gameTimer) override;

    void OnCollisionEnter2D(HitContact2D& contact) override;

    void OnTriggerStay(HitContact& contact) override;
    void OnTriggerExit(HitContact& contact) override;

    bool CanJump() const { return m_canJump; }

    void ChangeDimention() { m_is2D = !m_is2D; }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 2D時の更新関数
    void Update2D(const GameTimer& timer);

    // 3D時の更新関数
    void Update3D(const GameTimer& timer);
};