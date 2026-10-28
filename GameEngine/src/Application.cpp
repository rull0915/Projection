/**
 * @file    Application.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 */

#include "pch.h"
#include "Application.h"

#include "GameEngine.h"
#include "System/Render/GraphicsManager.h"
#include "Scene/SceneManager.h"
#include "System/WindowManager.h"
#include "Input/KeyInput.h"

#include "System/PerformanceStats.h"

namespace RHEngine
{
	Application::Application(const ApplicationProps& props)
		: m_props{ props }
		, m_gameEngine{ std::make_unique<GameEngine>() }
		, m_deviceResources{ nullptr }
		, m_timer{}
	{
		// デバイスリソースの取得
		m_deviceResources = GraphicsManager::Instance().GetDeviceResources();

		// TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
		//   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
		//   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
		m_deviceResources->RegisterDeviceNotify(this);
	}

	Application::~Application()
	{}

	void Application::Initialize(HWND hwnd, int width, int height)
	{
		// デバイスリソースにウィンドウを渡す
		m_deviceResources->SetWindow(hwnd, width, height);

		// デバイスの作成
		m_deviceResources->CreateDeviceResources();
		m_deviceResources->CreateWindowSizeDependentResources();

		// ウィンドウマネージャーにサイズを渡す
		WindowManager::Instance().SetWindow(hwnd);
		WindowManager::Instance().SetSize(width, height);
		WindowManager::Instance().SetDefaultWindowSize(width, height);

		// ゲームエンジンの初期化
		m_gameEngine->Initialize(m_deviceResources, hwnd, false);

		// アプリ側の初期化
		OnInitialize();
	}

	void Application::Finalize()
	{
		SceneManager::Instance().Finalize();

		OnFinalize();
	}

	void Application::Tick()
	{
		m_gameEngine->BeginFrame();

		m_timer.Tick([&]()
			{
				Update();
			});

		Render();
	}

	void Application::OnWindowSizeChanged(int width, int height)
	{
		// デバイスリソースに通知
		m_deviceResources->WindowSizeChanged(width, height);

		// ウィンドウマネージャーに通知
		WindowManager::Instance().SetSize(width, height);

		// ゲームエンジンに通知
		m_gameEngine->OnWindowSizeChanged(width, height);
	}

	void Application::Update()
	{
		// 経過時間を取得
		float elapsedTime = float(m_timer.GetElapsedSeconds());

		// Alt+Enterでフルスクリーン化
		if (Input::Key::Get(Input::Key::Code::LeftAlt) && Input::Key::GetDown(Input::Key::Code::Enter))
		{
			WindowManager::Instance().SwitchScreenMode();
		}

		// パフォーマンスの更新
		PerformanceStats::Instance().Update(elapsedTime);

		// ゲームエンジンの更新
		m_gameEngine->Update(elapsedTime);

		// アプリ側の更新
		OnUpdate(elapsedTime);
	}

	void Application::Render()
	{
		// Don't try to render anything before the first Update.
		if (m_timer.GetFrameCount() == 0)
		{
			return;
		}

		Clear();

		m_deviceResources->PIXBeginEvent(L"Render");

		// ゲームエンジンの描画
		m_gameEngine->Render();

		m_deviceResources->PIXEndEvent();

		// Show the new frame.
		m_deviceResources->Present();
	}

	void Application::Clear()
	{
		// クリアイベントの開始
		m_deviceResources->PIXBeginEvent(L"Clear");

		// Clear the views.
		auto context = m_deviceResources->GetD3DDeviceContext();
		auto renderTarget = m_deviceResources->GetRenderTargetView();
		auto depthStencil = m_deviceResources->GetDepthStencilView();

		// 背景色を取得
		auto color = WindowManager::Instance().GetBackGroundColor();

		// RTVのリセット
		context->ClearRenderTargetView(renderTarget, color);

		// DSTのリセット
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// RenderTargetのリセット
		context->OMSetRenderTargets(1, &renderTarget, depthStencil);

		// Set the viewport.
		const auto viewport = m_deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		m_deviceResources->PIXEndEvent();
	}
}	// namespace RHEngine
