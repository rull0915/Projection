/**
 * @file    Player.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/29
 */

#include "pch.h"
#include "Player.h"

#include "Components/World/RigidBody/RigidBody.h"
#include "Components/World/RigidBody/RigidBody2D.h"

#include "Components/World/Collider/3D/ColliderBase.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"

#include "Input/InputSystem.h"
#include "Input/PadInput.h"

#include "Physics/HitContact.h"

#include "Settings/WorldSetting2D.h"

using namespace RHEngine;

Player::Player(IComponentOwner* owner)
	: WorldComponentBase(owner)
	, m_pTransform{ GetComponent<Transform>() }
	, m_is2D{ false }
	, m_canJump{ false }
{
}

Player::~Player()
{
}

void Player::Start()
{
}

void Player::Update(const GameTimer& gameTimer)
{
	if (m_is2D) Update2D(gameTimer);

	else Update3D(gameTimer);
}

void Player::OnTriggerStay(HitContact& contact)
{
	// 床にぶつかっていた場合
	if (contact.other->GetTag() == "Floor")
	{
		// 衝突した法線が上方向に近いかどうかを判定
		float val = DirectX::SimpleMath::Vector3::Up.Dot(-contact.normal);
		if (val >= CAN_JUMP_BORDER)
		{
			m_canJump = true;
		}
	}
}

void Player::OnTriggerExit(HitContact & contact)
{
	// 床にぶつかっていた場合
	if (contact.other->GetTag() == "Floor")
	{
		m_canJump = false;
	}
}

void Player::Update2D(const GameTimer& timer)
{
	timer;

	// 軸の向きを取得
	DirectX::SimpleMath::Vector3 xAxis = WorldSetting2D::Instance().GetXAxis();
	DirectX::SimpleMath::Vector3 yAxis = WorldSetting2D::Instance().GetYAxis();

	// ----- 移動 ----- //

	// 移動方向の入力を取得
	float x = static_cast<float>(Input::Custom::GetAxis("Horizontal"));

	if (auto* rb = GetComponent<RigidBody2D>())
	{
		rb->SetVelocity({ x * MOVE_SPEED, rb->GetVelocity().y });
	}

	// 入力があれば
	if (m_canJump && Input::Custom::GetButton("Jump"))
	{
		// 力を加える
		if (auto* rb = GetComponent<RigidBody2D>())
		{
			rb->AddForce(DirectX::SimpleMath::Vector2::UnitY * JUMP_POWER, ForceMode::Impulse);
		}
	}
}

void Player::Update3D(const GameTimer& timer)
{
	timer;

	// ----- 移動 ----- //

	// 移動方向の入力を取得
	float x = static_cast<float>(Input::Custom::GetAxis("Horizontal"));
	float z = static_cast<float>(Input::Custom::GetAxis("Vertical"));

	// 移動方向を作成
	DirectX::SimpleMath::Vector3 movingDirecion{ x * m_pTransform->GetRight() + z * m_pTransform->GetForward() };
	movingDirecion.y = 0;

	// 移動量を一定にするために正規化
	movingDirecion.Normalize();

	// 速度を算出
	DirectX::SimpleMath::Vector3 movingVelocity = movingDirecion * MOVE_SPEED;// *timer.GetElapsedTime();

	// 速度を変更
	if (auto* rb = GetComponent<RigidBody>())
	{
		rb->SetVelocity({ movingVelocity.x, rb->GetVelocity().y, movingVelocity.z });
	}

	// ----- ジャンプ ----- //

	// 入力があれば
	if (m_canJump && (Input::Custom::GetButtonDown("Jump")))
	{
		// 力を加える
		if (auto* rb = GetComponent<RigidBody>())
		{
			rb->AddForce(DirectX::SimpleMath::Vector3::Up * JUMP_POWER, ForceMode::Impulse);
		}
	}
}
