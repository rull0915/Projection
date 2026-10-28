/**
 * @file    PerformanceStats.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 *
 * @brief   パフォーマンスに関する数値を持つシングルトンクラス
 */

#pragma once

#include <cstdint>

namespace RHEngine
{
	/**
	 * @brief パフォーマンスに関する数値を持つ
	 * @TODO CPU使用率やメモリ割合などにも対応する
	 */
	class PerformanceStats
	{
	private:

		/// @name パフォーマンス値
		/// @{

		/** 
		 * @brief FPS値
		 * @details 1秒ごとに再計算をする
		 */
		float m_fps;

		/// @}

		/// @name 計測時の補助変数
		/// @{

		uint32_t m_frameCount;		///< 経過したフレーム数
		float m_timeAccumulator;	///< 経過した時間

		/// @}

	private:

		/// @name ライフサイクル
		/// @{

		PerformanceStats() = default;

		~PerformanceStats() = default;

		/// @}

		/**
		 * @brief インスタンスを取得する
		 * @return 静的なインスタンス
		 */
		static PerformanceStats& Instance()
		{
			static PerformanceStats instance;
			return instance;
		}

		/**
		 * @brief 各パフォーマンス値を計測する
		 * @param elapsedTime 経過時間
		 */
		void Update(float elapsedTime);

		/// @name ゲッター
		/// @{

		/** @brief FPS */
		float GetFPS() const { return m_fps; }

		/// @}

	private:

		/**
		 * @brief FPSの計測をする
		 * @param elapsedTime 経過時間
		 */
		void MeasureFPS(float elapsedTime);
	};
}	// namespace RHEngine
