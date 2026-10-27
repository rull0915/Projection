/**
 * @file    RigidBody.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/06
 *
 * @brief   3D物理挙動コンポーネント
 */

#pragma once

#include <SimpleMath.h>

#include "RigidBodyBase.h"

namespace RHEngine
{
	class GameObject;
	class Transform;
	class IComponentOwner;

	/**
	 * @brief 3D物理挙動コンポーネント
	 */
	class RigidBody : public RigidBodyBase
	{
	public:
		DECLARE_COMPONENT_TYPE(RigidBodyBase);

	private:

		/** @brief 速度 */
		DirectX::SimpleMath::Vector3 m_velocity;
			
		/** @brief 加速度 */
		DirectX::SimpleMath::Vector3 m_acceleration;

		/** @brief かかっている力 */
		DirectX::SimpleMath::Vector3 m_force;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		RigidBody(IComponentOwner* own);

		~RigidBody() = default;

		/// @}

		/**
		 * @brief 力から位置・速度・加速度を時間で積分して求める
		 * @param elapsedTime 1fの経過時間
		 */
		void Integrate(float elapsedTime) override;

		/**
		 * @brief 力を加える
		 * @param vec 加える力ベクトル
		 * @param mode 力の種類
		 * @param wakeUp スリープ状態を解除するかどうか
		 */
		void AddForce(DirectX::SimpleMath::Vector3 vec, ForceMode mode = ForceMode::Force, bool wakeUp = true);

		/// @name ゲッター
		/// @{
		
		/** @brief 速度 */
		DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }

		/** @brief 加速度 */
		DirectX::SimpleMath::Vector3 GetAcceleration() const { return m_acceleration; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 速度 */
		void SetVelocity(DirectX::SimpleMath::Vector3 v)
		{
			m_velocity = v;
			WakeUp();
		}

		/** @brief 加速度 */
		void SetAcceleration(DirectX::SimpleMath::Vector3 a)
		{
			m_acceleration = a;
			WakeUp();
		}

		/// @}
	};
} // namespace RHEngine
