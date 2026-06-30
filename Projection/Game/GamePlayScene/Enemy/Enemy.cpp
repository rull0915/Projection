//====================================================//
// ファイル名  : Enemy.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要       : 敵コンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Enemy.h"

#include "Physics/HitContact.h"
#include "Components/World/RigidBody/RigidBody.h"
#include "Components/World/RigidBody/RigidBody2D.h"

// ステート
#include "State/EnemyIdleState.h"
#include "State/3D/EnemyJumpState.h"
#include "State/3D/EnemyMoveState.h"
#include "State/2D/Enemy2DMoveState.h"
#include "State/2D/Enemy2DJumpState.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void Enemy::Awake()
{
	// ステートの追加

	// アイドル
	m_stateMachine.RegisterState(EnemyStateID::Idle, std::make_unique<EnemyIdleState>(this));

	// ジャンプ
	m_stateMachine.RegisterState(EnemyStateID::Jump, std::make_unique<EnemyJumpState>(this));
	m_stateMachine.RegisterState(EnemyStateID::Jump2D, std::make_unique<Enemy2DJumpState>(this));

	// 移動
	m_stateMachine.RegisterState(EnemyStateID::Move, std::make_unique<EnemyMoveState>(this));
	m_stateMachine.RegisterState(EnemyStateID::Move2D, std::make_unique<Enemy2DMoveState>(this));

	// 初期状態をアイドルに設定
	m_stateMachine.RequsetChangeState(EnemyStateID::Idle);

	// 物理マテリアルの設定
	m_physicsMaterial.SetBounciness(0.0f);
	m_physicsMaterial.SetBounceCombine(CombineMode::Minimum);

	if (auto cl = GetOwn()->GetComponentWithCategory(Category::Collider))
	{
		static_cast<ColliderBase*>(cl)->SetPhysicsMaterial(&m_physicsMaterial);
	}
}

void Enemy::Start()
{
	// トランスフォームを取得
	m_pTransform = GetComponent<Transform>();
}

void Enemy::Update(const GameTimer& gameTimer)
{
	// ステートマシンの更新
	m_stateMachine.Update(gameTimer);

	// もしIdle状態ではない場合
	if (m_stateMachine.GetCurrentStateType() != EnemyStateID::Idle)
	{
		if (m_is2D)
		{
			// RigidBody2Dを取得
			if (auto rb = GetComponent<RigidBody2D>())
			{
				// 動いていなければ
				if (rb->GetVelocity().LengthSquared() <= FLT_EPSILON)
				{
					// Idleへ戻す
					m_stateMachine.RequsetChangeState(EnemyStateID::Idle);
				}
			}
		}
		else
		{
			// RigidBodyを取得
			if (auto rb = GetComponent<RigidBody>())
			{
				// 動いていなければ
				if (rb->GetVelocity().LengthSquared() <= FLT_EPSILON)
				{
					// Idleへ戻す
					m_stateMachine.RequsetChangeState(EnemyStateID::Idle);
				}
			}
		}
	}
}

void Enemy::OnCollisionEnter(HitContact & hit)
{
	// 衝突したオブジェクトが候補点を持っていたら
	if (auto comp = hit.other->GetComponent<LandingCandidatePoints>())
	{
		// 衝突法線が上向きなら
		if (hit.normal.y < 0)
		{
			// 更新
			m_lastPoints = comp;

			m_isGround = true;
		}
	}

	// 持っていなければ
	else
	{
		// Idleへ
		m_stateMachine.RequsetChangeState(EnemyStateID::Idle);
	}
}

void Enemy::OnCollisionExit(HitContact & hit)
{
	// 衝突したオブジェクトが候補点を持っていたら
	if (auto comp = hit.other->GetComponent<LandingCandidatePoints>())
	{
		// 最新と一致していたら
		if (comp == m_lastPoints)
		{
			// リセット
			m_lastPoints = nullptr;

			m_isGround = false;
		}
	}
}

void Enemy::OnCollisionEnter2D(HitContact2D& hit)
{
	// 衝突したオブジェクトが候補点を持っていたら
	if (auto comp = hit.other->GetComponent<LandingCandidatePoints2D>())
	{
		// 衝突法線が上向きなら
		if (hit.normal.y < 0)
		{
			// 更新
			m_lastPoints2D = comp;

			m_isGround = true;
		}
	}

	// 持っていなければ
	else
	{
		// Idleへ
		m_stateMachine.RequsetChangeState(EnemyStateID::Idle);
	}
}

void Enemy::OnCollisionExit2D(HitContact2D & hit)
{
	// 衝突したオブジェクトが候補点を持っていたら
	if (auto comp = hit.other->GetComponent<LandingCandidatePoints2D>())
	{
		// 最新と一致していたら
		if (comp == m_lastPoints2D)
		{
			// リセット
			m_lastPoints2D = nullptr;

			m_isGround = false;
		}
	}
}
