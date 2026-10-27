/**
 * @file    PhysicsMaterial.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/27
 *
 * @brief   摩擦、反発の設定をする物理マテリアル
 */

#pragma once

#include <algorithm>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief 2つの値を組み合わせる際のモード
	 */
	enum class CombineMode
	{
		Average,	///< 平均値
		Minimum,	///< 最小値
		Multiply,	///< 乗算
		Maximum,	///< 最大値
	};

	/**
	 * @brief 物理マテリアル
	 */
	class PhysicsMaterial : public AssetBase
	{
	private:

		/** @brief 静的摩擦力 */
		float m_staticFriction;

		/** @brief 動摩擦力 */
		float m_dynamicFriction;

		/** @brief 反発係数 */
		float m_bounciness;

		/** @brief 摩擦の計算方法 */
		CombineMode m_frictionCombine;

		/** @brief 反発の計算方法 */
		CombineMode m_bounceCombine;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param sFric 静的摩擦力
		 * @param dFric 動摩擦力
		 * @param bounce 反発係数
		 * @param fMode 摩擦の計算方法
		 * @param bMode 反発の計算方法
		 */
		PhysicsMaterial(float sFric, float dFric, float bounce, CombineMode fMode, CombineMode bMode)
			: m_staticFriction{ sFric }
			, m_dynamicFriction{ dFric }
			, m_bounciness{ bounce }
			, m_frictionCombine{ fMode }
			, m_bounceCombine{ bMode }
		{
			ADD_PROPERTY(PhysicsMaterial, m_staticFriction);
			ADD_PROPERTY(PhysicsMaterial, m_dynamicFriction);
			ADD_PROPERTY(PhysicsMaterial, m_bounciness);
			ADD_PROPERTY(PhysicsMaterial, m_frictionCombine);
			ADD_PROPERTY(PhysicsMaterial, m_bounceCombine);
		};

		PhysicsMaterial()
			: PhysicsMaterial(0.6f, 0.6f, 0, CombineMode::Average, CombineMode::Average)
		{};

		/// @}

		/**
		 * @brief デフォルトの物理マテリアルを取得する
		 * @return デフォルト物理マテリアル
		 */
		static const PhysicsMaterial& GetDefault()
		{
			static PhysicsMaterial material;
			return material;
		}

		/// @name ゲッター
		/// @{

		/** @brief 静的摩擦力 */
		constexpr inline float GetStaticFriction() const { return m_staticFriction; }

		/** @brief 動摩擦力 */
		constexpr inline float GetDynamicFriction() const { return m_dynamicFriction; }

		/** @brief 反発係数 */
		constexpr inline float GetBounciness() const { return m_bounciness; }

		/** @brief 摩擦の計算方法 */
		constexpr inline CombineMode GetFrictionCombine() const { return m_frictionCombine; }

		/** @brief 反発の計算方法 */
		constexpr inline CombineMode GetBounceCombine() const { return m_bounceCombine; }

		/// @}

		/// @name セッター
		/// @{
		
		/** @brief 静的摩擦力 */
		inline void SetStaticFriction(float value) { m_staticFriction = std::max(value, 0.0f); }

		/** @brief 動摩擦力 */
		inline void SetDynamicFriction(float value) { m_dynamicFriction = std::max(value, 0.0f); }

		/** @brief 反発係数 */
		inline void SetBounciness(float value) { m_bounciness = std::clamp(value, 0.0f, 1.0f); }

		/** @brief 摩擦の計算方法 */
		inline void SetFrictionCombine(CombineMode mode) { m_frictionCombine = mode; }

		/** @brief 反発の計算方法 */
		inline void SetBounceCombine(CombineMode mode) { m_bounceCombine = mode; }
	};

	namespace Physics
	{
		/**
		 * @brief 優先度を取得する
		 * @param mode 取得対象のモード
		 * @return 優先度
		 */
		constexpr int GetPriority(CombineMode mode)
		{
			switch (mode)
			{
			case CombineMode::Average:  return 0;
			case CombineMode::Minimum:  return 1;
			case CombineMode::Multiply: return 2;
			case CombineMode::Maximum:  return 3;	///< 最優先
			}
			return -1;
		}

		/**
		 * @brief 2つの値を組み合わせて計算する
		 * @param valueA 1つ目の値
		 * @param valueB 2つ目の値
		 * @param modeA 1つ目の計算方法
		 * @param modeB 2つ目の計算方法
		 * @return 計算結果
		 */
		constexpr inline float GetValue(float valueA, float valueB, CombineMode modeA, CombineMode modeB)
		{
			// 優先度の高いモードを使用
			CombineMode mode = GetPriority(modeA) > GetPriority(modeB) ? modeA : modeB;

			switch (mode)
			{
				// 平均値
			case CombineMode::Average:
				return (valueA + valueB) / 2;
				// 最大値
			case CombineMode::Maximum:
				return std::max(valueA, valueB);
				// 最小値
			case CombineMode::Minimum:
				return std::min(valueA, valueB);
				// 乗算
			case CombineMode::Multiply:
				return valueA * valueB;
			default:
				return -1;
			}
		}
	}	// namespace Physics
} // namespace RHEngine
