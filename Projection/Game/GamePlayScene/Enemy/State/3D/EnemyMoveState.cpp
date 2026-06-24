//====================================================//
// ファイル名  : EnemyMoveState.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要       : 敵のジャンプステート
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EnemyMoveState.h"

#include "../../Enemy.h"
#include "GameLib/GameObject/Components/RigidBody/3D/RigidBody.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EnemyMoveState::Enter()
{
	// パスを取得
	const PathFollower::Path* path = GetOwner()->GetNowPath();

	// パスがなければIdleに戻す
	if (!path)
	{
		RequestChangeState(EnemyStateID::Idle);

		return;
	}

	// 最初の位置を取得
	DirectX::SimpleMath::Vector3 initPosition = GetOwner()->GetComponent<Transform>()->GetWorldPosition();

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
	if (auto rb = GetOwner()->GetComponent<RigidBody>())
	{
		// 速度を変更
		rb->SetVelocity(m_moveVec * Enemy::VELOCITY);
	}

	// Colliderを取得
	if (auto col = GetOwner()->GetComponent<BaseCollider>())
	{
		// 摩擦をなくす
		auto* mt = col->GetMutablePhysicsMaterial();

		mt->SetFrictionCombine(CombineMode::Minimum);
		mt->SetDynamicFriction(0);
		mt->SetStaticFriction(0);
	}
}

void EnemyMoveState::Update(const GameTimer& timer)
{
	// トランスフォームを取得
	Transform* pTransform = GetOwner()->GetComponent<Transform>();

	// 今の位置を取得
	DirectX::SimpleMath::Vector3 nowPosition = pTransform->GetWorldPosition();

	// 次の座標から目標値へのベクトル
	DirectX::SimpleMath::Vector3 toTarget = m_targetPosition - nowPosition;

	// 水平方向が目標地点にたどり着いていたら
	if (m_moveVec.Dot(toTarget) < 0)
	{
		// 地面にいれば
		if (GetOwner()->IsGround())
		{
			// ジャンプステートへの移行を要請
			RequestChangeState(EnemyStateID::Jump);
		}
	}
}

void EnemyMoveState::Exit()
{
	// Colliderを取得
	if (auto col = GetOwner()->GetComponent<BaseCollider>())
	{
		// 摩擦を戻す
		auto* mt = col->GetMutablePhysicsMaterial();

		mt->SetFrictionCombine(CombineMode::Average);
		mt->SetDynamicFriction(0.6f);
		mt->SetStaticFriction(0.6f);
	}

	// 速度をリセット
	if (auto rb = GetOwner()->GetComponent<RigidBody>())
	{
		// Y方向のみそのまま
		rb->SetVelocity({ 0, rb->GetVelocity().y, 0 });
	}
}
