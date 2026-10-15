//====================================================//
// ファイル名  : Application.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Application.h"

#include "GameEngine.h"
#include "System/Render/GraphicsManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

REngine::Application::Application(const ApplicationProps& props)
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

REngine::Application::~Application()
{
}

void REngine::Application::Initialize(HWND hwnd, int width, int height)
{
	// デバイスリソースにウィンドウを渡す
	m_deviceResources->SetWindow(hwnd, width, height);

	// デバイスの作成
	m_deviceResources->CreateDeviceResources();
	m_deviceResources->CreateWindowSizeDependentResources();

	// ゲームエンジンの初期化
	m_gameEngine->Initialize(m_deviceResources, hwnd, false);

	// アプリ側の初期化
	OnInitialize();
}

void REngine::Application::Tick()
{
	m_gameEngine->BeginFrame();

	m_timer.Tick([&]()
		{
			Update();
		});

	Render();
}

void REngine::Application::Update()
{
	// 経過時間を取得
	float elapsedTime = float(m_timer.GetElapsedSeconds());

	// ゲームエンジンの更新
	m_gameEngine->Update(elapsedTime);

	// アプリ側の更新
	OnUpdate(elapsedTime);
}

void REngine::Application::Render()
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

void REngine::Application::Clear()
{
	// クリアイベントの開始
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	// 背景色を取得
	auto color = REngine::WindowManager::Instance().GetBackGroundColor();

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
