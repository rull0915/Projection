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

void Enemy::Update(const GameTimer & gameTimer)
{
	// 道が存在していれば
	if (m_way.size() > 1)
	{
		// インターバル中なら
		if (m_isInterval)
		{
			// 目標地点まで移動する
			DirectX::SimpleMath::Vector3 position = m_pTransform->GetWorldPosition();
		}
	}
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
