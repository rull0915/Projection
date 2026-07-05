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
#include "System/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//
// コンストラクタ
SceneManager::SceneManager()
	: m_scenes{}
	, m_pCurrentScene{ nullptr }
	, m_pRequestedScene{ nullptr }
{
}

// デストラクタ
SceneManager::~SceneManager()
{
	// 実行中のシーンの終了処理
	if (m_pCurrentScene) m_pCurrentScene->BaseFinalize();
}

// シーンの登録
void SceneManager::RegisterScene(const std::string& sceneName, std::unique_ptr<Scene> scene)
{
	// シーンの名前でシーンを登録
	m_scenes.emplace(sceneName, std::move(scene));
}

// 更新処理
void SceneManager::Update(const GameTimer& gameTimer)
{
	// 遷移演出の更新
	m_transitionManager.Update(gameTimer);

	// シーンの更新
	if (m_pCurrentScene) m_pCurrentScene->BaseUpdate(gameTimer);
}

// 描画処理
void SceneManager::Render(Renderer& renderer)
{
	// シーンの描画処理
	if (m_pCurrentScene) m_pCurrentScene->BaseRender(renderer);

	// 描画開始
	renderer.Start(ResourceManager::Instance().GetResources()->GetD3DDeviceContext());

	// シーンのスクリーンへの描画処理
	if (m_pCurrentScene) m_pCurrentScene->BaseRenderOnScreen(renderer);

	// 遷移演出の描画
	m_transitionManager.Render(renderer);

	// 描画終了
	renderer.End();
}

// 開始シーンの設定
void SceneManager::SetStartScene(const std::string& startSceneName)
{
	RequestSceneChange(startSceneName);
}

void SceneManager::RequestSceneChange(const std::string& requestSceneName, std::unique_ptr<Transition::Base> out, std::unique_ptr<Transition::Base> in)
{
	// シーンが未登録なら
	if (m_scenes.count(requestSceneName) == 0) return;

	// 演出中なら
	if (m_transitionManager.IsTransitioning()) return;

	// 登録されたリクエストシーンを取得
	m_pRequestedScene = m_scenes[requestSceneName].get();

	// 遷移開始
	m_transitionManager.StartTrans(
		std::move(out), std::move(in),	// 演出を渡す	
		[this]()
		{
			ChangeScene();
		}
	);
}

void SceneManager::RequestSceneChange(const std::string& requestSceneName)
{
	// シーンが未登録なら
	if (m_scenes.count(requestSceneName) == 0) return;

	// 演出中なら
	if (m_transitionManager.IsTransitioning()) return;

	// 登録されたリクエストシーンを取得
	m_pRequestedScene = m_scenes[requestSceneName].get();

	// 遷移開始
	m_transitionManager.StartTrans(
		nullptr, nullptr,
		[this]()
		{
			ChangeScene();
		}
	);
}

// シーンの変更
void SceneManager::ChangeScene()
{
	// シーンのリクエスト要求がない？
	if (!m_pRequestedScene) return;

	// 実行中のシーンの終了処理
	if (m_pCurrentScene) m_pCurrentScene->BaseFinalize();

	// リクエスト要求のシーンへ移行
	m_pCurrentScene = m_pRequestedScene;

	// シーンの初期化処理
	m_pCurrentScene->BaseInitialize();

	// リクエスト要求をクリア
	m_pRequestedScene = nullptr;
}
