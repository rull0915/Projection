/**
 * @file    RigidBodyBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/22
 *
 * @brief   リジッドボディ基底クラス
 */

#pragma once

#include <cstdint>

#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	/**
	 * @brief 力のモード
	 */
	enum class ForceMode
	{
		Force,
		Acceleration,
		Impulse,
		VelocityChange,
	};

	/**
	 * @brief 2D3D両方で使用するパラメータをまとめたRigidBodyの基底
	 */
	class RigidBodyBase : public WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

		/** @brief スリープへ移行する時間 (秒) */
		static constexpr float SLEEP_BORDER = 2.0f;

	private:
		/** @brief トランスフォームポインタ */
		Transform* m_pTransform;

		/** @brief 質量 */
		float m_mass;

		/** @brief 質量の逆数 力計算時などに使用 */
		float m_invMass;

		/** @brief 静止している時間 */
		float m_stoppingTime;

		/** @brief 速度の減衰率 (空気抵抗) */
		float m_linearDamping;

		/** @brief 固定されているかどうか */
		bool m_isStatic;

		/** @brief 重力の影響を受けるかどうか */
		bool m_useGravity;

		/** @brief スリープ状態かどうか */
		bool m_isSleep;

		/** @brief バージョン番号 */
		mutable uint32_t m_latestVersion;

	public:

		/// @name ライフサイクル
		/// @{

		RigidBodyBase(IComponentOwner* own)
			: WorldComponentBase(own)
			, m_pTransform{ GetComponent<Transform>() }
			, m_mass{ 1.0f }
			, m_invMass{ 1.0f / m_mass }
			, m_linearDamping{ 0.0f }
			, m_isStatic{ false }
			, m_useGravity{ true }
			, m_isSleep{ false }
			, m_stoppingTime{ 0 }
			, m_latestVersion{ 0 }
		{
			ADD_PROPERTY(RigidBodyBase, m_mass);
			ADD_PROPERTY(RigidBodyBase, m_linearDamping);
			ADD_PROPERTY(RigidBodyBase, m_isStatic);
			ADD_PROPERTY(RigidBodyBase, m_useGravity);
		}

		virtual ~RigidBodyBase() = default;

		/// @}

		/**
		 * @brief 位置・速度・加速度を積分する
		 * @param elapsedTime 1fの経過時間
		 */
		virtual void Integrate(float elapsedTime) = 0;

		/**
		 * @brief スリープ状態を解除する
		 */
		void WakeUp()
		{
			SetSleep(false);
			SetStoppingTime(0);
		}

		/**
		 * @brief 最新バージョンに更新する
		 */
		inline void ApplyVersion() const { m_latestVersion = m_pTransform->GetVersion(); }

		/**
		 * @brief GUI上での値変更時に逆数を再計算する
		 */
		void OnValidate() override { m_invMass = 1.0f / m_mass; }

		/** @brief Transformの状態変更を検知し起こす */
		void CheckWakeUp()
		{
			// トランスフォームの状態が変わっているか調べる
			if (m_latestVersion != m_pTransform->GetVersion())
			{
				ApplyVersion();
				WakeUp();
			}
		}

		/// @name ゲッター
		/// @{

		/** @brief 質量 */
		inline float GetMass() const { return m_mass; }

		/** @brief 質量の逆数 */
		inline float GetInvMass() const { return m_invMass; }

		/** @brief 固定かどうか */
		inline bool IsStatic() const { return m_isStatic; }

		/** @brief 重力の影響を受けるかどうか */
		inline bool IsUseGravity() const { return m_useGravity; }

		/** @brief スリープ状態かどうか */
		inline bool IsSleep() const { return m_isSleep; }

		/** @brief 速度の減衰率 (空気抵抗) */
		inline float GetLinearDamping() const { return m_linearDamping; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 質量 */
		void SetMass(float mass)
		{
			m_mass = mass;
			m_invMass = 1.0f / mass;
		}

		/** @brief 固定かどうか */
		void SetStatic(bool frag) { m_isStatic = frag; }

		/** @brief 重力の影響を受けるかどうか */
		void SetUseGravity(bool frag)
		{
			m_useGravity = frag;
			WakeUp();
		}

		/** @brief スリープかどうか */
		void SetSleep(bool frag) { m_isSleep = frag; }

		/** @brief 速度の減衰率 (空気抵抗) */
		void SetLinearDamping(float r) { m_linearDamping = std::max(0.0f, r); }

	protected:

		/** @brief Transformポインタを取得する */
		Transform* GetTransform() const { return m_pTransform; }

		/** @brief 停止している時間を取得する */
		float GetStoppingTime() const { return m_stoppingTime; }

		/** @brief 停止している時間を加算する */
		void AddStoppingTime(float time) { m_stoppingTime += time; }

		/** @brief 停止している時間を変更する */
		void SetStoppingTime(float time) { m_stoppingTime = time; }
	};
} // namespace RHEngine
