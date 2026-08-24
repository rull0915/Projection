//====================================================//
// ファイル名  : Player.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要       : プレイヤークラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
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

using namespace REngine;

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Player::Player(IComponentOwner* owner)
	: WorldComponentBase(owner)
	, m_pTransform{ GetComponent<Transform>() }
	// , m_material{ 0.2f, 0.1f, 0.0f, CombineMode::Minimum, CombineMode::Minimum }
	, m_is2D{ false }
	, m_canJump{ false }
	, m_lastPoints{ nullptr }
	, m_lastPoints2d{ nullptr }
{
}

Player::~Player()
{
}

void Player::Start()
{
	// 物理マテリアルを設定
	std::vector<ComponentBase*> colliders;
	GetOwn()->GetComponentsWithCategory(Category::Collider, colliders);

	// 全てのコライダー
	if (colliders.size() > 0)
	{
		for (auto collider : colliders)
		{
			// static_cast<ColliderBase*>(collider)->SetPhysicsMaterial(&m_material);
		}
	}

	// タグを設定
	GetOwn()->SetTag("Player");
}

void Player::Update(const GameTimer& gameTimer)
{
	if (m_is2D) Update2D(gameTimer);

	else Update3D(gameTimer);
}

void Player::OnCollisionEnter(HitContact& contact)
{
	// 候補点を持つオブジェクトなら
	if (auto comp = contact.other->GetComponent<LandingCandidatePoints>())
	{
		// 最新を更新
		m_lastPoints = comp;
	}
}

// 2Dコライダーに当たった時
void Player::OnCollisionEnter2D(HitContact2D& contact)
{
	// 候補点を持つオブジェクトなら
	if (auto comp = contact.other->GetComponent<LandingCandidatePoints2D>())
	{
		// 最新を更新
		m_lastPoints2d = comp;
	}
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

	if (auto* col = GetComponent<ColliderBase2D>())
	{
		// col->SetPhysicsMaterial(&m_material);
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
	if (m_canJump && (Input::Custom::GetButton("Jump") || Input::Pad::Get(Input::State::Down, Input::Pad::Button::A)))
	{
		// 力を加える
		if (auto* rb = GetComponent<RigidBody>())
		{
			rb->AddForce(DirectX::SimpleMath::Vector3::Up * JUMP_POWER, ForceMode::Impulse);
		}
	}
}
