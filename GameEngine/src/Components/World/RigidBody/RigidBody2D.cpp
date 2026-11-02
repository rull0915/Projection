/**
 * @file    RigidBody2D.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/06
 */

#include "pch.h"
#include "Components/World/RigidBody/RigidBody2D.h"

#include "Settings/WorldSetting2D.h"

using namespace DirectX;

namespace RHEngine
{
	RigidBody2D::RigidBody2D(IComponentOwner* pOwner)
		: RigidBodyBase(pOwner)
		, m_velocity{ 0.0f }
		, m_acceleration{ 0.0f }
		, m_force{ 0.0f }
	{}

	void RigidBody2D::Integrate(float elapsedTime)
	{
		CheckWakeUp();

		// 非アクティブならスキップ
		if (!IsActive()) return;

		// スリープ中ならスキップ
		if (IsSleep()) return;

		// 速度を調べる
		if (m_velocity.LengthSquared() < 0.1f)
		{
			AddStoppingTime(elapsedTime);
		}
		else SetStoppingTime(0);

		// 一定時間止まっていたら
		if (GetStoppingTime() >= SLEEP_BORDER)
		{
			// スリープにセット
			SetSleep(true);
			m_velocity = SimpleMath::Vector2::Zero;
			m_force = SimpleMath::Vector2::Zero;
		}

		// 加速度を力から計算
		SimpleMath::Vector2 acceleration = m_force * GetInvMass();

		// 速度に反映
		m_velocity += acceleration * elapsedTime;

		// 速度の減衰
		m_velocity *= 1.0f / (1.0f + GetLinearDamping() * elapsedTime);

		// 座標に反映
		auto& world2D = WorldSetting2D::Instance();

		auto velocity3D =
			m_velocity.x * world2D.GetXAxis()
			+ m_velocity.y * world2D.GetYAxis();

		GetTransform()->AddWorldPosition(velocity3D * elapsedTime);

		// 貯まった力をリセット
		m_force = SimpleMath::Vector2::Zero;
	}

	void RigidBody2D::AddForce(DirectX::SimpleMath::Vector2 vec, ForceMode mode, bool wakeUp)
	{
		switch (mode)
		{
		case ForceMode::Force:
			m_force += vec;
			break;
		case ForceMode::Acceleration:
			m_force += vec * GetMass();
			break;
		case ForceMode::Impulse:
			m_velocity += vec * GetInvMass();
			break;
		case ForceMode::VelocityChange:
			m_velocity += vec;
			break;
		default:
			break;
		}

		if (wakeUp) WakeUp();
	}
}	// namespace RHEngine
