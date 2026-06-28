//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Renderer/Renderer.h"
#include "Transition/TransitionManager.h"

// 前方参照
class Scene;

// 前方参照
class Scene;

// シーンマネージャークラス
class SceneManager
{

private:

	// エイリアス宣言
	using SceneCollection = std::unordered_map<std::string, std::unique_ptr<Scene>>;

	// シーン群
	SceneCollection m_scenes;

	// 実行中のシーン
	Scene* m_pCurrentScene;

	// 要求されたシーン
	Scene* m_pRequestedScene;

	// 演出管理クラス
	TransitionManager m_transitionManager;

public:

	// コンストラクタ
	SceneManager();

	// デストラクタ
	~SceneManager();

	// シーンの登録
	void RegisterScene(const std::string& sceneName, std::unique_ptr<Scene> scene);

	// 更新処理
	void Update(const GameTimer& gameTimer);

	// 描画処理
	void Render(Renderer& renderer);

	// 開始シーンの設定
	void SetStartScene(const std::string& startSceneName);

	// シーンの変更

	// Out|In別の演出指定版
	void RequestSceneChange(
		const std::string& requestSceneName,	// 遷移先のシーン名
		std::unique_ptr<Transition::Base> out,	// Out演出
		std::unique_ptr<Transition::Base> in	// In演出
	);

	// 演出なし版
	void RequestSceneChange(
		const std::string& requestSceneName		// 遷移先のシーン名
	);

private:

	// シーンの変更
	void ChangeScene();

};
