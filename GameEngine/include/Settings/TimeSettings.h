/**
 * @file    TimeSettings.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/03
 *
 * @brief   時間関連の設定をまとめたクラスです。
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 時間設定
	 */
	class TimeSettings
	{
	private:
	
		/** @brief タイムスケール */
		float m_timeScale;

	private:

		/// @name ライフサイクル
		/// @{

		TimeSettings()
			: m_timeScale{ 1.0f }
		{};

		~TimeSettings() = default;

		/// @}

	public:

		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static TimeSettings& Instance()
		{
			static TimeSettings instance;
			return instance;
		}

		/// @name ゲッター
		/// @{

		/** @brief タイムスケール */
		float GetTimeScale() const { return m_timeScale; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief タイムスケール */
		void SetTimeScale(float scale) { m_timeScale = scale; }

		/// @}
	};
}	// namespace RHEngine
