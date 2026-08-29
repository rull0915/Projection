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

#define IS_COMPONENT(Enemy)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

#include "Components/LandingCandidatePoints.h"
#include "Components/LandingCandidatePoints2D.h"

#include "PathFollower.h"

#include "Common/State/StateMachine.h"
#include "State/EnemyStateBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Enemy : public REngine::WorldComponentBase
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
	REngine::Transform* m_pTransform;   

	// ステートマシン本体
	REngine::StateMachine<EnemyStateID> m_stateMachine;
	
	// 最後に着地したポイント
	LandingCandidatePoints* m_lastPoints;
	LandingCandidatePoints2D* m_lastPoints2D;

	// パスを管理するオブジェクト
	PathFollower m_pathFollower;

	// 物理マテリアル
	// REngine::PhysicsMaterial m_physicsMaterial;

	// 着地フラグ
	bool m_isGround;

	// 2Dかどうか
	bool m_is2D;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------

	Enemy(REngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_pTransform{ nullptr }
		, m_stateMachine{}
		, m_lastPoints{ nullptr }
		, m_isGround{ false }
		, m_is2D{ false }
		, m_lastPoints2D{ nullptr }
	{
	}

	~Enemy() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	static REngine::Component::TypeId StaticTypeId() {
		static char id; return &id;
	} bool IsTypeOf(REngine::Component::TypeId id) const override {
		return id == StaticTypeId() || REngine::WorldComponentBase::IsTypeOf(id);
	} void CollectTypeIds(std::vector<REngine::Component::TypeId>& out) const override {
		out.push_back(StaticTypeId()); REngine::WorldComponentBase::CollectTypeIds(out);
	}

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;

	void OnCollisionEnter(REngine::HitContact& hit) override;
	void OnCollisionExit(REngine::HitContact& hit) override;

	void OnCollisionEnter2D(REngine::HitContact2D& hit) override;
	void OnCollisionExit2D(REngine::HitContact2D& hit) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// 最後に触れた候補点
	LandingCandidatePoints* GetLandingPoints() const { return m_lastPoints; }
	LandingCandidatePoints2D* GetLandingPoints2D() const { return m_lastPoints2D; }

	// 現在のパス
	const PathFollower::Path* GetNowPath() const { return m_pathFollower.GetNowPath(); }
	const PathFollower::Path2D* GetNowPath2D() const { return m_pathFollower.GetNowPath2D(); }

	// 道の更新が必要かどうか
	bool NeedUpdateWay() const { return m_pathFollower.NeedUpdateWay(); }

	// 現在のステート
	EnemyStateID GetNowState() const { return m_stateMachine.GetCurrentStateType(); }

	// 着地しているか
	bool IsGround() const { return m_isGround; }

	// 次元
	bool Is2D() const { return m_is2D; }

	// 物理マテリアル
	// REngine::PhysicsMaterial* GetPhysicsMaterial() { return &m_physicsMaterial; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	// 経路
	void SetWay(const std::vector<PathFollower::Path>& way) { m_pathFollower.SetWay(way); }
	void SetWay(const std::vector<PathFollower::Path2D>& way) { m_pathFollower.SetWay(way); }

	// 次の道へ移行する関数
	void ToNextPath() { m_pathFollower.ToNextPath(); }

	// 2Dかどうか
	void SetIs2D(bool is2D) 
	{
		m_is2D = is2D; 

		// Idle状態へ
		m_stateMachine.RequsetChangeState(EnemyStateID::Idle);
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------


private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};
