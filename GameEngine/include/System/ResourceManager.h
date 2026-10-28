/**
 * @file    ResourceManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/31
 *
 * @brief   リソース関連のゲーム全体で共有する変数をまとめたクラス
 */

#pragma once

#include <memory>
#include <Audio.h>

namespace RHEngine
{
	/**
	 * @brief リソース関連のゲーム全体で使用する変数をまとめた
	 * @TODO 現状オーディオエンジンしか管理していないので別のクラスに委譲する
	 */
	class ResourceManager
	{
	private:

		/** @brief オーディオエンジン */
		std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

	private:

		/// @name ライフサイクル
		/// @{

		ResourceManager();

		~ResourceManager();
		
		/// @}

	public:

		/**
		 * @brief インスタンス取得
		 * @return 静的なインスタンス
		 */
		static ResourceManager& Instance()
		{
			static ResourceManager instance;
			return instance;
		}

		/**
		 * @brief オーディオエンジンの初期化
		 */
		void Initialize();

		/**
		 * @brief オーディオエンジンの更新
		 */
		void Update();

		/**
		 * @brief オーディオエンジンを取得する
		 */
		DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	};
} // namespace RHEngine
