//====================================================//
// ファイル名  : EnemyJumpState.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要       : 敵のジャンプステート
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EnemyJumpState.h"

#include "GameLib/GameObject/Components/RigidBody/3D/RigidBody.h"
#include "../../Enemy.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EnemyJumpState::Enter()
{
	// ジャンプ処理
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
	m_targetPosition = path->goal;

	// 移動ベクトルを算出
	m_moveVec = m_targetPosition - initPosition;

	m_moveVec.y = 0;

	// 移動する距離を算出
	float len = m_moveVec.Length();

	// 正規化
	m_moveVec /= len;

	// スピードを算出
	m_moveSpeed = len / path->time;

	// Rigidbodyを取得
	if (auto rb = GetOwner()->GetComponent<RigidBody>())
	{
		// 速度を変更
		rb->SetVelocity(m_moveVec * m_moveSpeed);

		// ジャンプ用の力を加える
		rb->AddForce(DirectX::SimpleMath::Vector3::UnitY * Enemy::JUMP_IMPLUSE, ForceMode::Impulse);
	}

	m_arrivedXZ = false;
	m_arrivedY = false;
}

void EnemyJumpState::Update(const GameTimer& timer)
{
	// トランスフォームを取得
	Transform* pTransform = GetOwner()->GetComponent<Transform>();

	// 今の位置を取得
	DirectX::SimpleMath::Vector3 nowPosition = pTransform->GetWorldPosition();

	// 今の座標から目標値へのベクトル
	DirectX::SimpleMath::Vector3 toTarget = m_targetPosition - nowPosition;

	// 水平方向が目標地点にたどり着いていたら
	if (!m_arrivedXZ && m_moveVec.Dot(toTarget) < 0)
	{
		// 水平方向の到着フラグを立てる
		m_arrivedXZ = true;
	}
	
	// 着地していれば
	if (!m_arrivedY && GetOwner()->IsGround())
	{
		// 垂直方向のフラグを立てる
		m_arrivedY = true;
	}

	// 垂直、水平どちらも移動が終了していれば
	if (m_arrivedXZ && m_arrivedY)
	{
		// 移動ステートへの移行を要請
		RequestChangeState(EnemyStateID::Idle);
	}
}

void EnemyJumpState::Exit()
{
	// 次のパスへ
	GetOwner()->ToNextPath();

	// 速度をリセット
	if (auto rb = GetOwner()->GetComponent<RigidBody>())
	{
		// Y方向のみそのまま
		rb->SetVelocity({ 0, rb->GetVelocity().y, 0 });
	}
}
