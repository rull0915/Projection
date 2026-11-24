/**
 * @file    GameEngine.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/16
 */

#include "pch.h"
#include "GameEngine.h"

#include "Scene/SceneManager.h"

#ifdef ENGINE_GUI
#include "Editor/Editor/ImguiManager.h"
#endif // ENGINE_GUI

#include "System/EngineInitializer.h"
#include "Assets/System/AssetInitializer.h"
#include "Input/KeyInput.h"
#include "Input/MouseInput.h"
#include "Input/PadInput.h"
#include "Assets/Types/Model/DefaultMeshManager.h"
#include "Common/LogSystem/Logger.h"

namespace RHEngine
{
	GameEngine::GameEngine(GraphicsManager& graphicsManager)
		: m_modeChange{ false }
		, m_gameTimer{ std::make_unique<GameTimer>() }
		, m_assetManager{}
		, m_graphicManager{ graphicsManager }
		, m_renderer{ m_assetManager, graphicsManager }
		, m_shadowRenderer{ m_assetManager, graphicsManager }
#ifdef ENGINE_GUI
		, m_editor{}
		, m_sceneEdit{}
#endif // ENGINE_GUI
	{
	}

	void GameEngine::Initialize(HWND window, bool edit)
	{
		// デバイスリソースの取得
		auto* pDeviceResources = m_graphicManager.GetDeviceResources();

		if (!pDeviceResources)
		{
			LOG_FATAL("DeviceResources is null");
			return;
		}

		// デバイス コンテキストの取得
		auto* pDevice = pDeviceResources->GetD3DDevice();
		auto* pContext = pDeviceResources->GetD3DDeviceContext();

		// 取得できなかった場合致命的なエラーとして終了
		if (!pDevice) {
			LOG_FATAL("Direct3D device is null");
		}
		if (!pContext) {
			LOG_FATAL("Direct3D device context is null");
		}

		// シーンマネージャーの初期化
		SceneManager::Instance().Initialize(m_assetManager, m_graphicManager);

		// ゲームエンジンの初期化
		EngineInitializer::EngineInitialize();

		// アセットの初期化
		AssetInitializer::AssetInitialize(m_assetManager);

		// アセット管理クラスの初期化
		m_assetManager.Initialize(pDevice, L"Resources");

		// デフォルトメッシュの作成
		DefaultMeshManager::Instance().Initialize(pDevice);

		// 描画クラスの初期化
		m_renderer.Initialize();
		m_shadowRenderer.Initialize();

#ifdef ENGINE_GUI

		// imguiの初期化
		ImguiManager::Initialize(window, pDevice, pContext);

		// エディターの生成
		m_editor = std::make_unique<SceneEditor>(SceneManager::Instance().GetCurrentScene(), m_assetManager, m_graphicManager.GetDeviceResources()->GetD3DDevice());

		m_sceneEdit = edit;

		if (m_sceneEdit)
		{
			// 初期化
			m_editor->Initialize();
		}
#endif // ENGINE_GUI
	}

	void GameEngine::BeginFrame()
	{
		// ロガーにフレームの更新を通知
		Logger::Instance().NextFrame();

#ifdef ENGINE_GUI

		if (m_modeChange)
		{
			if (m_sceneEdit)
				EndEditor("Title");
			else
				StartEditor();

			m_modeChange = !m_modeChange;
		}

		if (m_sceneEdit)
		{
			// ImGuiの更新
			ImguiManager::Update();
		}

#endif // ENGINE_GUI
	}

	void GameEngine::Update(float elapsedTime)
	{
		// 入力情報の更新
		Input::Key::Update();
		Input::Mouse::Update();
		Input::Pad::Update();

		// アセットマネージャーの更新
		m_assetManager.Update();

		// タイマーの更新
		m_gameTimer->Update(elapsedTime);

		// 各シーンの更新
		SceneManager::Instance().Update(*m_gameTimer);

#ifdef ENGINE_GUI

		if (m_sceneEdit)
		{
			// エディターの更新
			m_editor->Update(*m_gameTimer);
		}
		// Ctrl + Shift + Eキーでエディットとプレイの切り替え
		if (
			Input::Key::Get(Input::Key::Code::LeftControl) &&
			Input::Key::Get(Input::Key::Code::LeftShift) &&
			Input::Key::GetDown(Input::Key::Code::E))
		{
			m_modeChange = true;
		}

#endif // ENGINE_GUI
	}

	void GameEngine::OnWindowSizeChanged(int width, int height)
	{
		// シーンマネージャーに通知
		SceneManager::Instance().OnWindowSizeChanged(width, height);

#ifdef ENGINE_GUI

		// シーンエディタに通知
		if (m_editor) m_editor->OnWindowSizeChanged(width, height);

#endif // ENGINE_GUI
	}

	void GameEngine::Render()
	{
		// 現在のシーンの描画
		SceneManager::Instance().Render(m_renderer, m_shadowRenderer);

#ifdef ENGINE_GUI

		if (m_sceneEdit)
		{
			// エディターの描画
			m_editor->Render(m_renderer);

			// Imguiの描画
			RHEngine::ImguiManager::Render();
		}

#endif // ENGINE_GUI
	}

	void GameEngine::Finalize()
	{
		// シーンの終了
		SceneManager::Instance().Finalize();

		// デフォルトメッシュの破棄
		DefaultMeshManager::Instance().Shutdown();

#ifdef ENGINE_GUI

		// Imguiの終了
		ImguiManager::Finalize();

#endif // ENGINE_GUI
	}

#ifdef ENGINE_GUI

	void GameEngine::StartEditor()
	{
		if (!m_editor)
		{
			LOG_ERROR("Cannot start editor: SceneEditor is not initialized");
			return;
		}

		m_sceneEdit = true;

		// 初期化
		m_editor->Initialize();
	}

	void GameEngine::EndEditor(std::string initSceneName)
	{
		if (!m_editor) return;

		m_sceneEdit = false;

		m_editor->Finalize();

		SceneManager::Instance().RequestSceneChange(initSceneName);
	}

#endif // ENGINE_GUI

}	// namespace RHEngine
