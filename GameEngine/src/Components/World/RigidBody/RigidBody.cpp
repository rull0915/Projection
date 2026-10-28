/**
 * @file    RigidBody.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/06
 */

#include "pch.h"
#include "Components/World/RigidBody/RigidBody.h"

using namespace DirectX;

namespace RHEngine
{
	RigidBody::RigidBody(IComponentOwner* pOwner)
		: RigidBodyBase(pOwner)
		, m_velocity{ 0.0f }
		, m_acceleration{ 0.0f }
		, m_force{ 0.0f }
	{}

	void RigidBody::Integrate(float elapsedTime)
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
			m_velocity = SimpleMath::Vector3::Zero;
			m_force = SimpleMath::Vector3::Zero;
		}

		// 加速度を力から計算
		SimpleMath::Vector3 acceleration = m_force * GetInvMass();

		// 速度に反映
		m_velocity += acceleration * elapsedTime;

		// 速度の減衰
		m_velocity *= 1.0f / (1.0f + GetLinearDamping() * elapsedTime);

		// 座標に反映
		GetTransform()->AddWorldPosition({ m_velocity * elapsedTime });

		// 貯まった力をリセット
		m_force = SimpleMath::Vector3::Zero;
	}

	void RigidBody::AddForce(DirectX::SimpleMath::Vector3 vec, ForceMode mode, bool wakeUp)
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
