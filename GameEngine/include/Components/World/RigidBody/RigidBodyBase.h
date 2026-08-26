//====================================================//
// ファイル名   : RigidBodyBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/22
//
// 概要 : リジッドボディ基底クラス
//
// 更新履歴 :
// 2026/04/22 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

namespace REngine
{
	//====================================================//
	// 列挙型宣言
	//====================================================//
	enum class ForceMode
	{
		Force,
		Acceleration,
		Impulse,
		VelocityChange,
	};

	//====================================================//
	// クラス宣言
	//====================================================//
	class RigidBodyBase : public WorldComponentBase
	{
	public:
		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------
		static constexpr float SLEEP_BORDER = 2.0f;

	private:
		// トランスフォームポインタ
		Transform* m_pTransform;

		// 質量
		float m_mass, m_invMass;    // 質量 / 質量の逆数

		// 静止している時間
		float m_stoppingTime;

		// 減衰率
		float m_linearDamping;  // 減衰率

		// フラグ
		bool m_isStatic;    // 固定
		bool m_useGravity;  // 重力の影響

		bool m_isSleep;     // スリープフラグ

		// 最新のバージョン
		mutable uint32_t m_latestVersion;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
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
			ADD_PROPERTY(m_mass);
			ADD_PROPERTY(m_linearDamping);
			ADD_PROPERTY(m_isStatic);
			ADD_PROPERTY(m_useGravity);
		}

		virtual ~RigidBodyBase() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(RigidBodyBase, WorldComponentBase)

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		inline float GetMass() const { return m_mass; }
		inline float GetInvMass() const { return m_invMass; }
		inline bool IsStatic() const { return m_isStatic; }
		inline bool IsUseGravity() const { return m_useGravity; }
		inline bool IsSleep() const { return m_isSleep; }
		inline float GetLinearDamping() const { return m_linearDamping; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------
		void SetMass(float mass)
		{
			m_mass = mass;
			m_invMass = 1.0f / mass;
		}
		void SetStatic(bool frag) { m_isStatic = frag; }
		void SetUseGravity(bool frag)
		{
			m_useGravity = frag;
			WakeUp();
		}
		void SetSleep(bool frag) { m_isSleep = frag; }
		void SetLinearDamping(float r) { m_linearDamping = std::max(0.0f, r); }

		void CheckWakeUp()
		{
			// トランスフォームの状態が変わっているか調べる
			if (m_latestVersion != m_pTransform->GetVersion())
			{
				ApplyVersion();
				WakeUp();
			}
		}

		//-----------------------------------------------------
		// その他関数
		//-----------------------------------------------------
		virtual void Integrate(float elapsedTime) = 0;

		void WakeUp()
		{
			SetSleep(false);
			SetStoppingTime(0);
		}

		inline void ApplyVersion() const { m_latestVersion = m_pTransform->GetVersion(); }

		void OnValidate() override
		{
			// 逆数の再計算
			m_invMass = 1.0f / m_mass;
		}

		// protected関数群
	protected:
		Transform* GetTransform() const { return m_pTransform; }

		float GetStoppingTime() const { return m_stoppingTime; }
		void AddStoppingTime(float time) { m_stoppingTime += time; }
		void SetStoppingTime(float time) { m_stoppingTime = time; }
	};
} // namespace REngine
