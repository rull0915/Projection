//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Transition/TransitionManager.h"

namespace REngine
{
	// 前方参照
	class Scene;
	class AssetManager;

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

		// 描画処理
		void Render(Renderer& renderer);

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

	private:

		// シーンの変更
		void ChangeScene();
	};
}	// namespace REngine
