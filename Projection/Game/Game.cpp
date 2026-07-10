//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

// 各システム管理クラス
#include "Debug/DebugManager.h"			// デバッグ	
#include "System/WindowManager.h"		// ウィンドウ	
#include "System/ResourceManager.h"		// リソース
#include "Editor/Editor/ImguiManager.h"	// imgui

// 各プロジェクト初期化
#include "System/EngineInitializer.h"			// エンジン部分	
#include "Editor/System/EditorInitializer.h"	// エディター	部分
#include "GameInitializer.h"					// ゲーム部分	

// 入力
#include "Input/KeyInput.h"				// キー	
#include "Input/MouseInput.h"			// マウス

// 各シーン
#include "EditScene/TestPlayScene.h"
#include "EditScene/EditScene.h"

#include "GamePlayScene/GamePlayScene.h"
#include "TitleScene/TitleScene.h"

// その他
#include "Common/Random.h"
#include "GameLib/Common/ResourceReader.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
	: m_fps{}
	, m_timeAccumulator{}
	, m_frameCount{}
	, m_renderer{}
	, m_gameTimer{}
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
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
	// Imguiの終了
	ImguiManager::Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// ゲームエンジンの初期化
	EngineInitializer::EngineInitialize();

	// ゲームエディタの初期化
	EditorInitializer::Initialize();

	// ゲームの初期化
	GameInitializer::Initialize();

	// 乱数の初期化
	Random::Init();

	// 背景色の設定
	WindowManager::Instance().SetBackGroundColor({ 0.3f, 0.6f, 0.8f, 1.0f });

	// ====== シーンの登録 ====== //
	m_sceneManager.RegisterScene("Edit", std::make_unique<EditScene>(this));
	m_sceneManager.RegisterScene("TestPlay", std::make_unique<TestPlayScene>(this));

	m_sceneManager.RegisterScene("GamePlay", std::make_unique<GamePlayScene>(this));
	m_sceneManager.RegisterScene("Title", std::make_unique<TitleScene>(this));

	// 開始時のシーンを設定
	m_sceneManager.SetStartScene("Title");

	// ====== リソースの追加 ====== //

	// テクスチャ
	ResourceReader::ReadTextures(L"Resources/Textures");

	// 音
	ResourceReader::ReadSounds(L"Resources/Sounds");

	// フォント
	ResourceReader::ReadFonts(L"Resources/Fonts");

	// モデル
	ResourceReader::ReadModels(L"Resources/Models");

	// オブジェクト
	ResourceReader::ReadObjects(L"Resources/Objects");

	// ========================== //

	// imguiの初期化
	auto* device = ResourceManager::Instance().GetResources()->GetD3DDevice();
	auto* context = ResourceManager::Instance().GetResources()->GetD3DDeviceContext();
	ImguiManager::Initialize(window, device, context);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	// Imguiの更新
	ImguiManager::Update();

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

	// 入力情報の更新
	Input::Key::Update();
	Input::Mouse::Update();

	// デバッグマネージャーの更新
	DebugManager::Instance().Update(elapsedTime);

	// リソースマネージャーの更新
	ResourceManager::Instance().Update();

	// タイマーの更新
	m_gameTimer.Update(elapsedTime);

	// 各シーンの更新
	if (
		!DebugManager::Instance().IsGameStop() ||     // ゲーム停止中ではない 
		 DebugManager::Instance().IsStepUpdate()      // ステップ実行フレーム
		) 
		m_sceneManager.Update(m_gameTimer);
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
	auto context = m_deviceResources->GetD3DDeviceContext();

	context;

	// 描画の開始 ----------------------------------------

	// 現在のシーンの描画
	m_sceneManager.Render(m_renderer);

	// 描画の終了 ----------------------------------------

	m_deviceResources->PIXEndEvent();

	// Imguiの描画
	ImguiManager::Render();

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

	auto color = WindowManager::Instance().GetBackGroundColor();
	
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
	width = WindowManager::Instance().GetWidth();
	height = WindowManager::Instance().GetHeight();
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto states = m_deviceResources->GetCommonStates();

	// TODO: Initialize device dependent objects here (independent of window size).

	// 描画クラスの初期化
	m_renderer.Initialize(device, context, states);

	// リソースマネージャの初期化
	ResourceManager::Instance().Initialize(m_deviceResources.get());
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.
}

void Game::TitleNameUpdate(float elapsedTime)
{
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
