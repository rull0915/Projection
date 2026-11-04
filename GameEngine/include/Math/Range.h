/**
 * @file    Range.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 *
 * @brief   数値を範囲に制限する
 */

#pragma once

#include <algorithm>

namespace RHEngine
{
	/**
	 * @brief 範囲内にfloatを制限する
	 */
	class Range
	{
	private:

		/** @brief 許容範囲の最小値 */
		const float MIN;

		/** @brief 許容範囲の最大値 */
		const float MAX;

		/** @brief 実際の値 */
		float m_value;	

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param a 範囲のボーダー
		 * @param b もう一方の範囲のボーダー
		 */
		Range(float a, float b)
			: MIN{ std::min(a, b) }
			, MAX{ std::max(a, b) }
			, m_value{ (a + b) / 2 }
		{}

		~Range() = default;

		/// @}

		/**
		 * @details 代入演算子でクランプすることで通常のfloat変数と同様に扱えるようにする
		 * @param o 代入するfloat値
		 */
		Range& operator=(const float& o)
		{
			m_value = std::clamp(o, MIN, MAX);
			return *this;
		}

		/**
		 * @param rhs 代入するもう一方の範囲クラス
		 */
		Range& operator=(const Range& rhs)
		{
			if (this != &rhs)
			{
				// 値のみをコピーする
				m_value = std::clamp(rhs.m_value, MIN, MAX);
			}
			return *this;
		}

		/**
		 * @brief floatへの暗黙変換を許可
		 */
		operator float() const { return m_value; }

		/// @name ゲッター
		/// @{

		/** @brief 最小値 */
		float GetMin() const { return MIN; }

		/** @brief 最大値 */
		float GetMax() const { return MAX; }

		/** @brief 実際の値 */
		float GetValue() const { return m_value; }

		/// @}
	};
}	// namespace RHEngine
