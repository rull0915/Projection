/**
 * @file    SoundManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/01
 *
 * @brief   音コンポーネント管理クラス
 */

#pragma once

#include "../ReserveContainer.h"
#include "Components/Both/Sounds/AudioSource.h"
#include "Components/World/Sounds/AudioListener.h"

namespace RHEngine
{
	/**
	 * @brief 音コンポーネントを管理する
	 */
	class SoundManager : public ReserveContainer<AudioSource>
	{
	private:

		/** @brief 使用中のリスナー */
		AudioListener* m_listener;

	public:

		/// @name ライフサイクル
		/// @{

		SoundManager();

		~SoundManager() = default;

		/// @}

		/**
		 * @brief 3Dオーディオの設定を行う
		 */
		void Update();

		/**
		 * @brief リスナーコンポーネントを設定する
		 * @param l 
		 */
		void SetListener(AudioListener* pListener)
		{
			// 2人目は登録不可
			if (m_listener) return;

			m_listener = pListener;
		}

		/**
		 * @brief リスナーを削除する
		 * @param pListener 対象のリスナー
		 */
		void RemoveListener(AudioListener* pListener) { if (m_listener == pListener) m_listener = nullptr; }
	};
}	// namespace RHEngine
