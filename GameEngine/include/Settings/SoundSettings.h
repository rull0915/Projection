//====================================================//
// ファイル名   : SoundSettings.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要 : 音の設定項目
//
// 更新履歴 :
// 2026/07/08 新規作成
//====================================================//

#pragma once

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SoundSettings
	{
	public:

		enum class Channel : unsigned char
		{
			BGM = 0,	// 音楽
			SE = 1,	// 効果音
		};

	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// マスターの音量
		float m_masterVolume;

		// 各チャンネルの音量
		float m_bgmVolume;	// BGM
		float m_seVolume;	// SE

	private:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SoundSettings()
			: m_masterVolume{ 1.0f }
			, m_bgmVolume{ 1.0f }
			, m_seVolume{ 1.0f }
		{};
		~SoundSettings() = default;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// シングルトン化
		static SoundSettings& Instance()
		{
			static SoundSettings instance;
			return instance;
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// マスター
		float GetMasterVolume() const { return m_masterVolume; }

		// BGM
		float GetBGMVolume() const { return m_bgmVolume; }

		// SE
		float GetSEVolume() const { return m_seVolume; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// マスター
		void SetMasterVolume(float v) { m_masterVolume = v; }

		// BGM
		void SetBGMVolume(float v) { m_bgmVolume = v; }

		// SE
		void SetSEVolume(float v) { m_seVolume = v; }
	};
}	// namespace REngine
