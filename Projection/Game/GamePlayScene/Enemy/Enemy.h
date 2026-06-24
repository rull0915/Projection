//====================================================//
// ファイル名   : Enemy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 敵コンポーネント
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

#include "Components/LandingCandidatePoints.h"

#include "PathFollower.h"

#include "GameLib/Common/State/StateMachine.h"
#include "State/EnemyStateBase.h"
#include "GameLib/GameObject/Components/Collider/PhysicsMaterial.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Enemy : public Component<Enemy, ComponentID::Enemy>
{
public:
    //-----------------------------------------------------
    // constexpr宣言
    //-----------------------------------------------------
    static constexpr float VELOCITY = 5.0f;
    static constexpr float JUMP_IMPLUSE = 15.0f;

private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // トランスフォームのポインタ
    Transform* m_pTransform;   

    // ステートマシン本体
    StateMachine<EnemyStateID> m_stateMachine;
    
    // 最後に着地したポイント
    LandingCandidatePoints* m_lastPoints;

    // パスを管理するオブジェクト
    PathFollower m_pathFollower;

    // 物理マテリアル
    PhysicsMaterial m_physicsMaterial;

    // 着地フラグ
    bool m_isGround;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------

    Enemy(IComponentOwner* owner)
        : Component(owner)
        , m_pTransform{ nullptr }
        , m_stateMachine{}
        , m_lastPoints{ nullptr }
        , m_isGround{ false }
    {
    }

    ~Enemy() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void Update(const GameTimer& gameTimer) override;

    void OnCollisionEnter(HitContact& hit) override;
    void OnCollisionExit(HitContact& hit) override;

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    // 最後に触れた候補点
    LandingCandidatePoints* GetLandingPoints() const { return m_lastPoints; }

    // 現在のパス
    const PathFollower::Path* GetNowPath() const { return m_pathFollower.GetNowPath(); }

    // 道の更新が必要かどうか
    bool NeedUpdateWay() const { return m_pathFollower.NeedUpdateWay(); }

    // 現在のステート
    EnemyStateID GetNowState() const { return m_stateMachine.GetCurrentStateType(); }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

    // 経路
    void SetWay(const std::vector<PathFollower::Path>& way) { m_pathFollower.SetWay(way); }

    // 次の道へ移行する関数
    void ToNextPath() { m_pathFollower.ToNextPath(); }

    // 着地しているか
    bool IsGround() const { return m_isGround; }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------


private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};
