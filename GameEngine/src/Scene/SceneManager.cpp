//====================================================//
// ファイル名  : SceneManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/02/17
//
// 概要        :  シーン管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

#include "Editor/Loader/ObjectLoader.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//
	// コンストラクタ
	SceneManager::SceneManager()
		: m_currentScene{ std::make_unique<Scene>() }
		, m_pRequestedScene{ "" }
		, m_transitionManager{}
	{}

	// デストラクタ
	SceneManager::~SceneManager()
	{
		// 実行中のシーンの終了処理
		if (m_currentScene) m_currentScene->Finalize();
	}

	// シーンの登録
	void SceneManager::RegisterScene(const std::string& sceneName, const std::wstring& sceneFile)
	{
		// シーンの名前でシーンを登録
		m_sceneMap.insert({ sceneName, sceneFile });
	}

	// 更新処理
	void SceneManager::Update(const GameTimer& gameTimer)
	{
		// 遷移演出の更新
		m_transitionManager.Update(gameTimer);

		// シーンの更新
		if (m_currentScene) m_currentScene->Update(gameTimer);
	}

	// 描画処理
	void SceneManager::Render(Renderer& renderer)
	{
		// シーンの描画処理
		if (m_currentScene) m_currentScene->Render(renderer);

		// シーンのスクリーンへの描画処理
		if (m_currentScene) m_currentScene->RenderOnScreen(renderer);

		// 描画終了
		renderer.End();

		// 遷移演出の描画
		m_transitionManager.Render(renderer);

		// 描画終了
		renderer.End();
	}

	void SceneManager::Finalize()
	{
		// シーンの解放
		m_currentScene = nullptr;
	}

	// 開始シーンの設定
	void SceneManager::SetStartScene(const std::string& startSceneName)
	{
		RequestSceneChange(startSceneName);
	}

	void SceneManager::RequestSceneChange(const std::string& requestSceneName, std::unique_ptr<Transition::Base> out, std::unique_ptr<Transition::Base> in)
	{
		// シーンが未登録なら
		if (m_sceneMap.find(requestSceneName) == m_sceneMap.end()) return;

		// 演出中なら
		if (m_transitionManager.IsTransitioning()) return;

		// 登録されたリクエストシーンを取得
		m_pRequestedScene = requestSceneName;

		// 遷移開始
		m_transitionManager.StartTrans(
			std::move(out), std::move(in), [this]() { ChangeScene(); }
		);
	}

	// シーンの変更
	void SceneManager::ChangeScene()
	{
		// シーンのリクエスト要求がなければ
		if (m_pRequestedScene.empty()) return;

		// 実行中のシーンの終了処理
		if (m_currentScene) m_currentScene->Finalize();

		// パスの取得
		auto it = m_sceneMap.find(m_pRequestedScene);

		// 未登録なら読み込まない
		if (it != m_sceneMap.end())
		{
			// リクエスト要求のシーンへ移行
			ObjectLoader::LoadSceneFromFile(it->second, m_currentScene.get());
		}

		// シーンの初期化処理
		m_currentScene->Initialize();

		// リクエスト要求をクリア
		m_pRequestedScene = "";
	}
}	// namespace REngine
