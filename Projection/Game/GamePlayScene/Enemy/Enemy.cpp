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

#include "GameLib/GameObject/Managers/HitContact.h"
#include "GameLib/GameObject/Components/RigidBody/3D/RigidBody.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void Enemy::Awake()
{
	// トランスフォームを取得
	m_pTransform = GetComponent<Transform>();
}

void Enemy::Start()
{
}

void Enemy::Update(const GameTimer& gameTimer)
{
	Update3D();
}

void Enemy::OnCollisionEnter(HitContact & hit)
{
	// 衝突したオブジェクトが候補点を持っていたら
	if (auto comp = hit.other->GetComponent<LandingCandidatePoints>())
	{
		// 更新
		m_lastPoints = comp;
	}
}

void Enemy::Update3D()
{
	// 道が存在していれば
	if (m_way.size() > 0)
	{
		// 速度を変更
		if (auto rb = GetComponent<RigidBody>())
		{
			rb->SetVelocity({ m_velocity.x, rb->GetVelocity().y, m_velocity.z });
		}

		// 位置を取得
		DirectX::SimpleMath::Vector3 position = m_pTransform->GetWorldPosition();

		// 目標地点に近づいたら
		if ((position - m_target).LengthSquared() <= 0.1f)
		{
			// 目標地点へ補正する
			m_pTransform->SetWorldPosition(m_target);

			// 次の動きへ移行
			ToNextMove();
		}
	}
}

void Enemy::Update2D()
{}

void Enemy::ToNextMove()
{
	// インターバルからなら
	if (m_isInterval)
	{
		// インデックスが範囲外なら何もしない
		if (m_nowIndex < 0 || m_nowIndex >= m_way.size()) return;

		// 非インターバルへ
		m_isInterval = false;

		// ジャンプ
		if (auto rb = GetComponent<RigidBody>())
		{
			rb->AddForce(DirectX::SimpleMath::Vector3::UnitY * JUMP_IMPLUSE, ForceMode::Impulse);
		}

		// 速度を変更
		Path next = m_way[m_nowIndex];

		m_velocity = (next.goal - next.start) / next.time;

		m_velocity.y = 0;
	}
	// 非インターバルからなら
	else
	{
		// インターバルへ
		m_isInterval = true;

		// ネクストが設定されていたら
		if (m_nextWay.size() > 0)
		{
			// 道を変更する
 			ChangeWay();
		}

		m_nowIndex++;

		// インデックスが範囲外なら何もしない
		if (m_nowIndex < 0 || m_nowIndex >= m_way.size()) return;
		
		// 速度を設定する
		DirectX::SimpleMath::Vector3 position = m_pTransform->GetWorldPosition();

		m_velocity = m_way[m_nowIndex].start - position;

		m_velocity.Normalize();

		m_velocity *= VELOCITY;
	}
}
