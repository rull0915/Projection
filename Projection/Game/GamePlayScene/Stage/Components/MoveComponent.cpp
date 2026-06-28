//====================================================//
// ファイル名  : MoveComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要       : コンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "MoveComponent.h"

#include "Components/World/Transform/Transform.h"
#include "Physics/HitContact.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void MoveComponent::Awake()
{}

void MoveComponent::Start()
{
	// 最初の座標を取得
	m_initPoint = GetComponent<Transform>()->GetWorldPosition();

	m_sumTime = 0;
}

void MoveComponent::Update(const GameTimer & gameTimer)
{
	// 時間を加算
	m_sumTime += gameTimer.GetElapsedTime();

	// 位置を設定
	GetComponent<Transform>()->SetWorldPosition(m_initPoint + m_moveFunc(m_sumTime));

	// テスト
	//GetComponent<Transform>()->AddLocalEulerAngle({ 0, gameTimer.GetElapsedTime(), 0 });
}

void MoveComponent::OnCollisionEnter(HitContact& contact)
{
	if (contact.other->GetTag() == "Player")
	{
		// 上方向の面に当たっていれば
		float up = DirectX::SimpleMath::Vector3::UnitY.Dot(contact.normal);

		if (up > BORDER)
		{
			contact.other->GetComponent<Transform>()->SetParent(this->GetComponent<Transform>());
		}
	}
}

void MoveComponent::OnCollisionExit(HitContact & contact)
{
	if (contact.other->GetTag() == "Player")
	{
		contact.other->GetComponent<Transform>()->SetParent(nullptr);
	}
}
