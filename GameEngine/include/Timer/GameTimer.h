/**
 * @file    GameTimer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/04
 *
 * @brief   各オブジェクトに渡すゲーム用のタイマークラス
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief ゲーム全体で使うタイマー
	 * @details タイムスケールを扱う
	 */
	class GameTimer
	{
		/** @brief タイムスケールを反映した総経過時間 */
		float m_time;

		/** @brief リアルタイムの総経過時間 */
		float m_unscaledTime;

		/** @brief タイムスケールを反映した1f経過時間 */
		float m_elapsedTime;

		/** @brief リアルタイムの1f経過時間 */
		float m_unscaledElapsedTime;

	public:

		/// @name ライフサイクル
		/// @{

		GameTimer()
			: m_time{ 0.0f }
			, m_unscaledTime{ 0.0f }
			, m_elapsedTime{ 0.0f }
			, m_unscaledElapsedTime{ 0.0f }
		{}

		~GameTimer() = default;

		/// @}

		/**
		 * @brief タイマーを更新する
		 * @param elapsedTime 経過時間
		 */
		void Update(float elapsedTime);
		
		/// @name ゲッター
		/// @{

		/** @brief タイムスケールを反映した1f経過時間 */
		float GetElapsedTime() const { return m_elapsedTime; }

		/** @brief リアルタイムの1f経過時間 */
		float GetUnScaledElapsedTime() const { return m_unscaledElapsedTime; }

		/** @brief タイムスケールを反映した総経過時間 */
		float GetTime() const { return m_time; }

		/** @brief リアルタイムの総経過時間 */
		float GetUnScaledTime() const { return m_unscaledTime; }

		/// @}
	};
}	// namespace RHEngine
