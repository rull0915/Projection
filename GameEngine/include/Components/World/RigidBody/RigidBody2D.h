//====================================================//
// ファイル名   : RigidBody2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/06
//
// 概要 : RigidBody2Dクラス 質量や速度などを管理します
//
// 更新履歴 :
// 2026/04/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "RigidBodyBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class RigidBody2D : public RigidBodyBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 速度/加速度
		DirectX::SimpleMath::Vector2 m_velocity, m_acceleration;

		// かかっている力
		DirectX::SimpleMath::Vector2 m_force;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		RigidBody2D(IComponentOwner* own);

		~RigidBody2D() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Integrate(float elapsedTime);

		void AddForce(DirectX::SimpleMath::Vector2 vec, ForceMode mode = ForceMode::Force, bool wakeUp = true)
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


		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		DirectX::SimpleMath::Vector2 GetForce() const { return m_force; }
		DirectX::SimpleMath::Vector2 GetVelocity() const { return m_velocity; }
		DirectX::SimpleMath::Vector2 GetAcceleration() const { return m_acceleration; }

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<RigidBody2D>();
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetForce(DirectX::SimpleMath::Vector2 f) { m_force = f; }
		void SetVelocity(DirectX::SimpleMath::Vector2 v) { m_velocity = v; }
		void SetAcceleration(DirectX::SimpleMath::Vector2 a) { m_acceleration = a; }

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	};
} // namespace REngine
