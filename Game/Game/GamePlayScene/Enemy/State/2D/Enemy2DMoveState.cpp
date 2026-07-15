//====================================================//
// ファイル名  : Enemy2DMoveState.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要       : 敵の2D移動ステート
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Enemy2DMoveState.h"

#include "../../Enemy.h"
#include "Components/World/RigidBody/RigidBody2D.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"

#include "Settings/WorldSetting2D.h"

using namespace REngine;

//====================================================//
// 関数の実体宣言
//====================================================//

void Enemy2DMoveState::Enter()
{
	// パスを取得
	const PathFollower::Path2D* path = GetOwner()->GetNowPath2D();

	// パスがなければIdleに戻す
	if (!path)
	{
		RequestChangeState(EnemyStateID::Idle);

		return;
	}
	
	// 2次元世界の設定を取得
	auto& world2D = WorldSetting2D::Instance();

	// 最初の位置を取得
	DirectX::SimpleMath::Vector2 initPosition = world2D.World3DToLocal2D(GetOwner()->GetComponent<Transform>()->GetWorldPosition());

	// 目標地点を取得
	m_targetPosition = path->start;

	// 移動ベクトルを算出
	m_moveVec = m_targetPosition - initPosition;

	m_moveVec.y = 0;

	// 移動する距離を算出
	float len = m_moveVec.Length();

	// 正規化
	m_moveVec /= len;

	// RigidBodyを取得
	if (auto rb = GetOwner()->GetComponent<RigidBody2D>())
	{
		// 速度を変更
		rb->SetVelocity(m_moveVec * Enemy::VELOCITY);
	}

	// Colliderを取得
	if (auto col = GetOwner()->GetComponent<ColliderBase2D>())
	{
		// 摩擦をなくす
		auto* mt = col->GetMutablePhysicsMaterial();

		// なければ設定
		if (mt)
		{
			mt->SetFrictionCombine(CombineMode::Minimum);
			mt->SetDynamicFriction(0);
			mt->SetStaticFriction(0);
		}
	}
}

void Enemy2DMoveState::Update(const GameTimer& timer)
{
	timer;

	// トランスフォームを取得
	Transform* pTransform = GetOwner()->GetComponent<Transform>();
		
	// 2次元世界の設定を取得
	auto& world2D = WorldSetting2D::Instance();

	// 今の位置を取得
	DirectX::SimpleMath::Vector2 nowPosition = world2D.World3DToLocal2D(pTransform->GetWorldPosition());

	// 次の座標から目標値へのベクトル
	DirectX::SimpleMath::Vector2 toTarget = m_targetPosition - nowPosition;

	// 水平方向が目標地点にたどり着いていたら
	if (m_moveVec.Dot(toTarget) < 0)
	{
		// 地面にいれば
		if (GetOwner()->IsGround())
		{
			// 2次元ジャンプステートへの移行を要請
			RequestChangeState(EnemyStateID::Jump2D);
		}
	}

	// RigidBodyを取得
	if (auto rb = GetOwner()->GetComponent<RigidBody2D>())
	{
		DirectX::SimpleMath::Vector2 vel =
		{
			m_moveVec.x * Enemy::VELOCITY,
			rb->GetVelocity().y,
		};

		// 速度を変更
		rb->SetVelocity(vel);
	}
}

void Enemy2DMoveState::Exit()
{
	// Colliderを取得
	if (auto col = GetOwner()->GetComponent<ColliderBase2D>())
	{
		// 摩擦を戻す
		auto* mt = col->GetMutablePhysicsMaterial();

		if (mt)
		{
			mt->SetFrictionCombine(CombineMode::Average);
			mt->SetDynamicFriction(0.6f);
			mt->SetStaticFriction(0.6f);
		}
	}

	// 速度をリセット
	if (auto rb = GetOwner()->GetComponent<RigidBody2D>())
	{
		// Y方向のみそのまま
		rb->SetVelocity({ 0, rb->GetVelocity().y });
	}
}
