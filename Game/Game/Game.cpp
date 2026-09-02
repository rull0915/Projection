//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

// 各システム管理クラス
#include "System/GraphicsManager.h"	// グラフィック
#include "System/WindowManager.h"	// ウィンドウ	
#include "Scene/SceneManager.h"		// シーン

// 各プロジェクト初期化
#include "GameInitializer.h"		// ゲーム部分	

#include "Input/KeyInput.h"
#include "Input/PadInput.h"
#include "Scene/Transition/FadeTransition.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
	: m_fps{}
	, m_timeAccumulator{}
	, m_frameCount{}
	, m_gameEngine{ std::make_unique<REngine::GameEngine>() }
	, m_deviceResources{ nullptr }
{
	// デバイスリソースの取得
	m_deviceResources = GraphicsManager::Instance().GetDeviceResources();
	// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
	//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
	//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
	m_deviceResources->RegisterDeviceNotify(this);
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{
	// ゲームエンジンの終了処理
	m_gameEngine->Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);

	// デバイスの作成
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// ゲームエンジンの初期化
	m_gameEngine->Initialize(m_deviceResources, window);

	// ゲームの初期化
	GameInitializer::Initialize();

	// ====== シーンの登録 ====== //
	REngine::SceneManager::Instance().RegisterScene("Title", L"Resources/Scenes/TitleScene.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage1", L"Resources/Scenes/Stage1.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage2", L"Resources/Scenes/Stage2.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage3", L"Resources/Scenes/Stage3.scene");
	REngine::SceneManager::Instance().RegisterScene("Select", L"Resources/Scenes/SelectScene.scene");
	REngine::SceneManager::Instance().RegisterScene("Clear", L"Resources/Scenes/ClearScene.scene");
	REngine::SceneManager::Instance().RegisterScene("GameOver", L"Resources/Scenes/GameOverScene.scene");

	// 開始時のシーンを設定
	REngine::SceneManager::Instance().SetStartScene("Title");
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	m_gameEngine->BeginFrame();

	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.

	TitleNameUpdate(elapsedTime);

	// ゲームエンジンの更新
	m_gameEngine->Update(elapsedTime);

	// タイトルへ戻る処理
	// Todo : TGS用の特殊処理です 終わったら削除して下さい
	if (REngine::Input::Key::Get(REngine::Input::Key::Code::LeftControl) && REngine::Input::Key::GetDown(REngine::Input::Key::Code::T))
	{
		REngine::SceneManager::Instance().RequestSceneChange("Title", std::make_unique<REngine::Transition::Fade>(), std::make_unique<REngine::Transition::Fade>());
	}
	if (REngine::Input::Pad::Get(REngine::Input::Pad::Button::LStick) && REngine::Input::Pad::GetDown(REngine::Input::Pad::Button::RStick))
	{
		REngine::SceneManager::Instance().RequestSceneChange("Title", std::make_unique<REngine::Transition::Fade>(), std::make_unique<REngine::Transition::Fade>());
	}
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");

	// 描画の開始 ----------------------------------------

	// ゲームエンジンの描画
	m_gameEngine->Render();

	// 描画の終了 ----------------------------------------

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	auto color = REngine::WindowManager::Instance().GetBackGroundColor();
	
	context->ClearRenderTargetView(renderTarget, color);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	const auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
	const auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
	m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = REngine::WindowManager::Instance().GetWidth();
	height = REngine::WindowManager::Instance().GetHeight();
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	// TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.
}

void Game::TitleNameUpdate(float elapsedTime)
{
	elapsedTime;

	// デバッグなら
#ifdef _DEBUG

	// FPSの計算
	m_timeAccumulator += elapsedTime;
	++m_frameCount;

	// FPSを1秒ごとに更新
	if (m_timeAccumulator > 1.0f)
	{
		m_fps = static_cast<float>(m_frameCount) / m_timeAccumulator;

		// windowタイトルをFPSで更新
		HWND hwnd = m_deviceResources->GetWindow();

		if (hwnd)
		{
			std::wstring titleStr = TITLE_STRING;

			wchar_t title[128]{};

			swprintf_s(title, L"(FPS: %.1f)", m_fps);
			titleStr += title;

			SetWindowTextW(hwnd, titleStr.data());
		}

		m_frameCount = 0;
		m_timeAccumulator = 0;
	}

#endif
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion
