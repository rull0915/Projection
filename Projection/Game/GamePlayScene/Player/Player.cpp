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

#include "GameLib/GameObject/Components/RigidBody/3D/RigidBody.h"
#include "GameLib/GameObject/Components/RigidBody/2D/RigidBody2D.h"

#include "GameLib/GameObject/Components/Collider/3D/BaseCollider.h"

#include "GameLib/Input/KeyInput.h"

#include "GameLib/GameObject/Managers/HitContact.h"

#include "GameLib/GameObject/Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Player::Player(IComponentOwner* owner)
	: Component(owner)
	, m_pTransform{ GetComponent<Transform>() }
	, m_material{ 0.2f, 0.1f, 0.1f, CombineMode::Minimum, CombineMode::Minimum }
	, m_is2D{ false }
	, m_canJump{ false }
{
}

Player::~Player()
{
}

void Player::Start()
{
	// 物理マテリアルを設定
	std::vector<BaseCollider*> colliders;
	GetComponents<BaseCollider>(colliders);

	// 全てのコライダー
	if (colliders.size() > 0)
	{
		for (auto collider : colliders)
		{
			collider->SetPhysicsMaterial(&m_material);
		}
	}
}

void Player::Update(const GameTimer& gameTimer)
{
	if (m_is2D) Update2D(gameTimer);

	else Update3D(gameTimer);
}

// 2Dコライダーに当たった時
void Player::OnCollisionEnter2D(HitContact2D& contact)
{
	// 床にぶつかっていた場合
	if (contact.other->GetTag() == L"Floor")
	{
		// 衝突したオブジェクトに近づくように補正する

		// カメラ基準のZ座標を求める
		auto& world2D = WorldSetting2D::Instance();

		// 軸を取得する
		DirectX::SimpleMath::Vector3 xAxis = world2D.GetXAxis(), yAxis = world2D.GetYAxis();

		// 外積を使ってZ軸を算出
		DirectX::SimpleMath::Vector3 zAxis = xAxis.Cross(yAxis);
		zAxis.Normalize();

		// トランスフォームを取得
		Transform* targetTransform = contact.other->GetComponent<Transform>();
		Transform* ownTransform = GetComponent<Transform>();

		// 投影して座標を求める
		float targetZ = zAxis.Dot(targetTransform->GetWorldPosition());
		float ownZ = zAxis.Dot(ownTransform->GetWorldPosition());

		// 差分のみ座標を変化させる
		ownTransform->AddLocalPosition(zAxis * (targetZ - ownZ));
	}
}

void Player::OnTriggerStay(HitContact& contact)
{
	// 床にぶつかっていた場合
	if (contact.other->GetTag() == L"Floor")
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
	if (contact.other->GetTag() == L"Floor")
	{
		m_canJump = false;
	}
}

void Player::Update2D(const GameTimer& timer)
{
	// 軸の向きを取得
	DirectX::SimpleMath::Vector3 xAxis = WorldSetting2D::Instance().GetXAxis();
	DirectX::SimpleMath::Vector3 yAxis = WorldSetting2D::Instance().GetYAxis();

	// ----- 移動 ----- //

	// 移動方向の入力を取得
	float x = KeyInput::GetCustomInput(CustomType::Horizontal);

	if (auto* rb = GetComponent<RigidBody2D>())
	{
		rb->SetVelocity({ x * MOVE_SPEED, rb->GetVelocity().y });
	}

	// ジャンプ入力を取得
	float jumpInput = KeyInput::GetCustomInputDown(CustomType::Jump);

	// 入力があれば
	if (jumpInput != 0.0f)
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
	// ----- 移動 ----- //

	// 移動方向の入力を取得
	float x = KeyInput::GetCustomInput(CustomType::Horizontal);
	float z = KeyInput::GetCustomInput(CustomType::Vertical);

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

	// ジャンプ入力を取得
	float jumpInput = KeyInput::GetCustomInputDown(CustomType::Jump);

	// 入力があれば
	if (m_canJump && jumpInput != 0.0f)
	{
		// 力を加える
		if (auto* rb = GetComponent<RigidBody>())
		{
			rb->AddForce(DirectX::SimpleMath::Vector3::Up * JUMP_POWER, ForceMode::Impulse);
		}
	}
}
