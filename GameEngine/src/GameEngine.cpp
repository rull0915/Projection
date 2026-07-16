//====================================================//
// ファイル名  : GameEngine.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/16
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameEngine.h"

#include "Scene/SceneManager.h"
#include "Editor/Editor/ImguiManager.h"

#include "System/EngineInitializer.h"
#include "Common/Random.h"
#include "System/WindowManager.h"
#include "System/ResourceManager.h"
#include "Debug/DebugManager.h"
#include "Input/KeyInput.h"
#include "Input/MouseInput.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	GameEngine::GameEngine()
		: m_gameTimer{ std::make_unique<GameTimer>() }
		, m_renderer{ std::make_unique<Renderer>() }
		, m_editor{}
	{}

	void GameEngine::Initialize(DX::DeviceResources* deviceResources, HWND window)
	{
		// 乱数の初期化
		Random::Init();

		// デバイス コンテキストの取得
		auto* device = deviceResources->GetD3DDevice();
		auto* context = deviceResources->GetD3DDeviceContext();
		auto* states = deviceResources->GetCommonStates();

		// ゲームエンジンの初期化
		EngineInitializer::EngineInitialize();

		// 背景色の設定
		WindowManager::Instance().SetBackGroundColor({ 0.3f, 0.6f, 0.8f, 1.0f });

		// imguiの初期化
		ImguiManager::Initialize(window, device, context);

		// 描画クラスの初期化
		m_renderer->Initialize(device, context, states);

		// リソースマネージャの初期化
		ResourceManager::Instance().Initialize(deviceResources);

		// エディターの生成
		m_editor = std::make_unique<SceneEditor>(SceneManager::Instance().GetCurrentScene());

		// 初期化
	//	m_editor->Initialize();
	}

	void GameEngine::BeginFrame()
	{
#ifdef _DEBUG
		// ImGuiの更新
		ImguiManager::Update();
#endif
	}

	void GameEngine::Update(float elapsedTime)
	{
		// 入力情報の更新
		Input::Key::Update();
		Input::Mouse::Update();

		// デバッグマネージャーの更新
		DebugManager::Instance().Update(elapsedTime);

		// リソースマネージャーの更新
		ResourceManager::Instance().Update();

		// タイマーの更新
		m_gameTimer->Update(elapsedTime);
		// 各シーンの更新
		if (
			!DebugManager::Instance().IsGameStop() ||     // ゲーム停止中ではない 
			DebugManager::Instance().IsStepUpdate()      // ステップ実行フレーム
			)
			SceneManager::Instance().Update(*m_gameTimer);

#ifdef _DEBUG

		// エディターの更新
		m_editor->Update(*m_gameTimer);

		// Ctrl + Eキーでエディット
		if (Input::Key::Get(Input::State::Press, Input::Key::Code::LeftControl) &&
			Input::Key::Get(Input::State::Down, Input::Key::Code::E))
		{
			m_editor->Initialize();
		}
#endif
	}

	void GameEngine::Render()
	{
		// 現在のシーンの描画
		SceneManager::Instance().Render(*m_renderer);

#ifdef _DEBUG
		// エディターの描画
		m_editor->Render(*m_renderer);

		// Imguiの描画
		REngine::ImguiManager::Render();
#endif
	}

	void GameEngine::Finalize()
	{
		// シーンの終了
		SceneManager::Instance().Finalize();

		// Imguiの終了
		ImguiManager::Finalize();
	}
}	// namespace REngine
