//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Transition/TransitionManager.h"

namespace RHEngine
{
	// 前方参照
	class Scene;
	class AssetManager;
	
	class Renderer;
	class ShadowRenderer;

	// シーンマネージャークラス
	class SceneManager
	{
	private:

		// シーンの実体
		std::unique_ptr<Scene> m_currentScene;

		// シーン名とシーン情報のマップ
		std::unordered_map<std::string, std::wstring> m_sceneMap;

		// 要求されたシーン名
		std::string m_pRequestedScene;

		// 演出管理クラス
		TransitionManager m_transitionManager;

		// アセットマネージャー
		AssetManager* m_assetManager;

		// シーンを画面に描画するかどうか
		bool m_drawScene;

	private:

		// コンストラクタ
		SceneManager();

		// デストラクタ
		~SceneManager();

	public:

		// インスタンス
		static SceneManager& Instance()
		{
			static SceneManager instance;

			return instance;
		}

		// シーンの登録
		void RegisterScene(const std::string& sceneName, const std::wstring& sceneFile);

		// 初期化関数
		void Initialize(AssetManager& assetManager);

		// 更新処理
		void Update(const GameTimer& gameTimer);

		// ウィンドウサイズ変化時処理
		void OnWindowSizeChanged(int width, int height);

		// 描画処理
		void Render(Renderer& renderer, ShadowRenderer& shadowRenderer);

		// 終了処理
		void Finalize();

		// 開始シーンの設定
		void SetStartScene(const std::string& startSceneName);

		// シーンの変更
		void RequestSceneChange(
			const std::string& requestSceneName,	// 遷移先のシーン名
			std::unique_ptr<Transition::Base> out = nullptr,	// Out演出
			std::unique_ptr<Transition::Base> in = nullptr		// In演出
		);

		// シーンを取得する関数
		Scene* GetCurrentScene() const { return m_currentScene.get(); }

		// シーン描画設定の変更
		void SetDrawScene(bool flag) { m_drawScene = flag; }

	private:

		// シーンの変更
		void ChangeScene();
	};
}	// namespace RHEngine
