/**
 * @file    AudioClip.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/27
 *
 * @brief   音ファイルのアセット
 */

#pragma once

#include <memory>
#include <Audio.h>
#include <utility>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief 音ファイルのアセット
	 */
	class AudioClip : public AssetBase
	{
	private:

		/** @brief 音ファイル */
		std::unique_ptr<DirectX::SoundEffect> m_soundEffect;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param soundEffect 音ファイル
		 */
		AudioClip(std::unique_ptr<DirectX::SoundEffect> soundEffect)
			: m_soundEffect{ std::move(soundEffect) }
		{
		}
		~AudioClip() = default;

		/// @}

		/**
		 * @brief 音ファイルを取得する
		 * @return 音ファイルポインタ
		 */
		DirectX::SoundEffect* Get() const { return m_soundEffect.get(); }
	};
}	// namespace RHEngine
