/**
 * @file    RigidBody2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/06
 *
 * @brief   2D物理挙動コンポーネント
 */

#pragma once

#include <SimpleMath.h>

#include "RigidBodyBase.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief 2D物理挙動コンポーネント
	 */
	class RigidBody2D : public RigidBodyBase
	{
	public:
		DECLARE_COMPONENT_TYPE(RigidBodyBase);

	private:

		/** @brief 速度 */
		DirectX::SimpleMath::Vector2 m_velocity;

		/** @brief 加速度 */
		DirectX::SimpleMath::Vector2 m_acceleration;

		/** @brief かかっている力 */
		DirectX::SimpleMath::Vector2 m_force;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		RigidBody2D(IComponentOwner* own);

		~RigidBody2D() = default;

		/// @}

		/**
		 * @brief 力から位置・速度・加速度を時間で積分する
		 * @param elapsedTime 1fの経過時間
		 */
		void Integrate(float elapsedTime);

		void AddForce(DirectX::SimpleMath::Vector2 vec, ForceMode mode = ForceMode::Force, bool wakeUp = true);


		/// @name ゲッター
		/// @{

		/** @brief 速度 */
		DirectX::SimpleMath::Vector2 GetVelocity() const { return m_velocity; }

		/** @brief 加速度 */
		DirectX::SimpleMath::Vector2 GetAcceleration() const { return m_acceleration; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 速度 */
		void SetVelocity(DirectX::SimpleMath::Vector2 v)
		{
			m_velocity = v;
			WakeUp();
		}

		/** @brief 加速度 */
		void SetAcceleration(DirectX::SimpleMath::Vector2 a)
		{
			m_acceleration = a;
			WakeUp();
		}

		/// @}
	};
} // namespace RHEngine
