//====================================================//
// ファイル名  : Player.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Player.h"

#include "GameLib/Input/KeyInput.h"

#include "GameLib/GameObject/Components/Collider/3D/BaseCollider.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Player::Player(IComponentOwner* owner)
	: Component(owner)
	, m_pTransform{ GetComponent<Transform>() }
	, m_material{ 0.2f, 0.1f, 1.0f, CombineMode::Minimum, CombineMode::Minimum }
{
}

Player::~Player()
{
}

void Player::Start()
{
	// コライダーを取得しマテリアルを設定
	BaseCollider* collider = GetComponent<BaseCollider>();
	collider->SetPhysicsMaterial(&m_material);

//	GetComponent<RigidBody>()->SetLinearDamping(3);
}

void Player::Update(float elapsedTime)
{
	DirectX::SimpleMath::Vector3 direction;

	// 移動
	if (KeyInput::GetKey(KeyCode::A))
	{
		direction += -m_pTransform->GetRight();
	}
	if (KeyInput::GetKey(KeyCode::D))
	{
		direction += m_pTransform->GetRight();
	}
	if (KeyInput::GetKey(KeyCode::W))
	{
		direction += m_pTransform->GetForward();
	}
	if (KeyInput::GetKey(KeyCode::S))
	{
		direction += -m_pTransform->GetForward();
	}
	// y方向を消す
	direction.y = 0;
	direction.Normalize();

	if (KeyInput::GetKey(KeyCode::LeftShift))
	{
		m_pTransform->AddLocalPosition({ 0, -MOVE_SPEED * elapsedTime, 0 });
	}
	if (KeyInput::GetKey(KeyCode::Space))
	{
		m_pTransform->AddLocalPosition({ 0, MOVE_SPEED * elapsedTime, 0 });
	}

	m_pTransform->AddLocalPosition(direction * elapsedTime * MOVE_SPEED);

	if (KeyInput::GetKey(KeyCode::Left))
	{
		GetComponent<Transform>()->AddLocalEulerAngle({ 0, elapsedTime, 0 });
	}
	if (KeyInput::GetKey(KeyCode::Right))
	{
		GetComponent<Transform>()->AddLocalEulerAngle({ 0, -elapsedTime, 0 });
	}
	if (KeyInput::GetKey(KeyCode::Down))
	{
		float rotX = GetComponent<Transform>()->GetLocalEulerAngle().x;
		float targetX = rotX - elapsedTime;
		targetX = MyMath::Clamp(targetX, -PI_F * (7.0f / 15), PI_F * (7.0f / 15));

		GetComponent<Transform>()->AddLocalEulerAngle({ targetX - rotX, 0, 0 });
	}
	if (KeyInput::GetKey(KeyCode::Up))
	{
		float rotX = GetComponent<Transform>()->GetLocalEulerAngle().x;
		float targetX = rotX + elapsedTime;
		targetX = MyMath::Clamp(targetX, -PI_F * (9.0f / 15), PI_F * (4.0f / 15));

		GetComponent<Transform>()->AddLocalEulerAngle({ targetX - rotX, 0, 0 });	
	}
}
