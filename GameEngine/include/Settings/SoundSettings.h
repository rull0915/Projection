/**
 * @file    SoundSettings.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/08
 *
 * @brief   音の設定項目
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 音設定項目
	 */
	class SoundSettings
	{
	public:
		/**
		 * @brief サウンドチャンネル列挙型
		 */
		enum class Channel : unsigned char
		{
			BGM = 0,	// 音楽
			SE = 1,	// 効果音
		};

	private:
		
		/** @brief マスターボリューム */
		float m_masterVolume;

		/** @brief BGMチャンネルのボリューム */
		float m_bgmVolume;

		/** @brief SEチャンネルのボリューム */
		float m_seVolume;

	private:

		/// @name ライフサイクル
		/// @{

		SoundSettings()
			: m_masterVolume{ 1.0f }
			, m_bgmVolume{ 1.0f }
			, m_seVolume{ 1.0f }
		{};

		~SoundSettings() = default;

		/// @}

	public:

		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static SoundSettings& Instance()
		{
			static SoundSettings instance;
			return instance;
		}

		/// @name ゲッター
		/// @{

		/** @brief マスターボリューム */
		float GetMasterVolume() const { return m_masterVolume; }

		/** @brief BGMボリューム */
		float GetBGMVolume() const { return m_bgmVolume; }

		/** @brief SEボリューム */
		float GetSEVolume() const { return m_seVolume; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief マスターボリューム */
		void SetMasterVolume(float v) { m_masterVolume = v; }

		/** @brief BGMボリューム */
		void SetBGMVolume(float v) { m_bgmVolume = v; }

		/** @brief SEボリューム */
		void SetSEVolume(float v) { m_seVolume = v; }

		/// @}
	};
}	// namespace RHEngine
