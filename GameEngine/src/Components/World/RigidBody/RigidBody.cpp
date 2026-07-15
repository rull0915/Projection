//====================================================//
// ファイル名  : RigidBody.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/06
//
// 概要        :  RigidBodyコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/RigidBody/RigidBody.h"

using namespace DirectX;

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	RigidBody::RigidBody(IComponentOwner* own)
		: RigidBodyBase(own)
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
}	// namespace REngine
