/**
 * @file    SceneManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/02/06
 *
 * @brief   シーン管理クラス
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Transition/TransitionManager.h"
#include "Transition/TransitionBase.h"

namespace RHEngine
{
	class Scene;
	class AssetManager;
	
	class Renderer;
	class ShadowRenderer;
	class GameTimer;

	/**
	 * @brief シーン管理者
	 */
	class SceneManager
	{
	private:

		/** @brief シーンの実体 */
		std::unique_ptr<Scene> m_currentScene;

		/** @brief シーン名とシーン情報のマップ */
		std::unordered_map<std::string, std::wstring> m_sceneMap;

		/** @brief 変更要求されたシーン名 */
		std::string m_pRequestedScene;

		/** @brief シーン遷移演出管理クラス */
		TransitionManager m_transitionManager;

		/** @brief アセットマネージャー */
		AssetManager* m_pAssetManager;

		/** @brief シーンを画面に描画するかどうか */
		bool m_drawScene;

	private:

		/// @name ライフサイクル
		/// @{

		SceneManager();

		~SceneManager();

		/// @}

	public:

		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static SceneManager& Instance()
		{
			static SceneManager instance;

			return instance;
		}

		/**
		 * @brief シーンを登録する
		 * @param sceneName シーン名
		 * @param sceneFile シーンファイルパス
		 */
		void RegisterScene(const std::string& sceneName, const std::wstring& sceneFile);

		/**
		 * @brief 初期化関数 シーンの生成を行う
		 * @param assetManager アセットマネージャー
		 */
		void Initialize(AssetManager& assetManager);

		/**
		 * @brief シーン及び遷移の更新を行う
		 * @param gameTimer 
		 */
		void Update(const GameTimer& gameTimer);

		/**
		 * @brief ウィンドウサイズの変化をシーンへ通知する
		 * @param width 変更後のウィンドウ幅
		 * @param height 変更後のウィンドウ高さ
		 */
		void OnWindowSizeChanged(int width, int height);

		/**
		 * @brief シーンの描画を行う
		 * @param renderer 描画管理者
		 * @param shadowRenderer シャドウパス管理者
		 */
		void Render(Renderer& renderer, ShadowRenderer& shadowRenderer);

		/**
		 * @brief シーンを解放する
		 */
		void Finalize();

		/**
		 * @brief 開始シーンを設定する
		 * @param startSceneName シーン名
		 */
		void SetStartScene(const std::string& startSceneName);

		/**
		 * @brief シーンの変更を要請する
		 * @param requestSceneName 要請後のシーン名
		 * @param out 画面を隠すシーン遷移の演出
		 * @param in 画面を開くシーン遷移の演出
		 */
		void RequestSceneChange(
			const std::string& requestSceneName,
			std::unique_ptr<Transition::Base> out = nullptr,
			std::unique_ptr<Transition::Base> in = nullptr
		);

		/**
		 * @brief 現在のシーンを取得する
		 * @return 実行中のシーンポインタ
		 */
		Scene* GetCurrentScene() const { return m_currentScene.get(); }

		/**
		 * @brief シーンを描画するフラグ
		 */
		void SetDrawScene(bool flag) { m_drawScene = flag; }

	private:

		/**
		 * @brief シーンを変更する
		 */
		void ChangeScene();
	};
}	// namespace RHEngine
