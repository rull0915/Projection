/**
 * @file    AudioListener.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   オーディオを受け取るコンポーネント
 */

#pragma once

#include <cstdint>
#include <memory>
#include <Audio.h>

#include "Components/World/WorldComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;
	class Transform;
	class GameTimer;

	/**
	 * @brief オーディオを受け取るコンポーネント
	 * @details AudioSourceが3Dを使用している場合、AudioListener基準で音量やパンが計算される
	 */
	class AudioListener : public WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:
		
		/** @brief トランスフォームポインタ */
		Transform* m_pTransform;

		/** @brief オーディオをリスナーの実体 */
		std::unique_ptr<DirectX::AudioListener> m_listener;

		/**
		 * @brief 現在のバージョン番号
		 * @details トランスフォームの変更を検知しリスナーの位置を変更するために使用
		 */
		uint32_t m_nowVersion;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		AudioListener(IComponentOwner* own);

		~AudioListener() = default;

		/// @}

		/** @brief コンポーネント追加時にリスナーの初期化を行う */
		void Awake() override;

		/** @brief トランスフォームの変更を検知しリスナーの設定を変える */
		void LateUpdate(const GameTimer& timer) override;

		/// @name ゲッター
		/// @{

		/** @brief リスナーの実体 */
		DirectX::AudioListener* GetListener() const
		{
			return m_listener.get();
		}

		/// @}
	};
} // namespace RHEngine
