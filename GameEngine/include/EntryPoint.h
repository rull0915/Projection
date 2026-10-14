//====================================================//
// ファイル名   : EntryPoint.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要 : EntryPointを定義するヘッダ
//
// 更新履歴 : 
// 2026/10/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

// Windows API
#include <Windows.h>

#include <DirectXMath.h>

// 標準ライブラリ
#include <iterator>
#include <memory>

// DirectXTK Input
#include <KeyBoard.h>
#include <GamePad.h>
#include <Mouse.h>

// REngine
#include "Application.h"
#include "System/WindowManager.h"
#include "Editor/Editor/ImguiManager.h"

//====================================================//
// 関数宣言
//====================================================//

// 外部で定義されたCreateApplicationを呼び出す
extern REngine::Application* REngine::CreateApplication();

namespace {
    std::unique_ptr<REngine::Application> g_app;
}

// Windows Procedure 前方宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// エントリポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// 使用しないパラメータ
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!DirectX::XMVerifyCPUSupport()) return 1;
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	// アプリケーションインスタンスを生成
	g_app.reset(REngine::CreateApplication());
	const auto& props = g_app->GetProps();

	// キーボードの作成
	std::unique_ptr<DirectX::Keyboard> keyboard = std::make_unique<DirectX::Keyboard>();

	// マウスの作成
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();

	// パッドの作成
	std::unique_ptr<DirectX::GamePad> gamePad = std::make_unique<DirectX::GamePad>();

	// ウィンドウクラスの生成・登録
	{
		// ウィンドウ情報の作成
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.lpszClassName = L"REngine_Window";
		if (!RegisterClassExW(&wcex)) return 1;

		// ウィンドウサイズの指定
		RECT rc = { 0, 0, static_cast<LONG>(props.width), static_cast<LONG>(props.height) };
		AdjustWindowRect(&rc, WS_MYSTYLE, FALSE);

		// ウィンドウの作成
		HWND hwnd = CreateWindowExW(0, L"REngine_Window", props.title.c_str(), WS_MYSTYLE,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
			nullptr, nullptr, hInstance,
			g_app.get());

		// 作成失敗したら終了
		if (!hwnd)
			return 1;

		ShowWindow(hwnd, nCmdShow);

		// マウスにウインドウハンドルを設定する
		mouse->SetWindow(hwnd);

		// アプリケーションの初期化
		g_app->Initialize(hwnd, props.width, props.height);
	}

	// メインループ
	MSG msg = {};

	// 終了メッセージが読み込まれるまでループ
	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			g_app->Tick();
		}
	}

	// 終了時処理
	g_app->OnFinalize();

	// アプリケーションのリセット
	g_app.reset();

	return static_cast<int>(msg.wParam);
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef ENGINE_GUI
	if (REngine::ImguiManager::ProcessMessage(hWnd, message, wParam, lParam))
	{
		return true;
	}
#endif // USE_IMGUi

	// 受け取ったメッセージによって処理を分岐
	switch (message)
	{

		// Window破棄時
	case WM_DESTROY:

		// ゲームを終了
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

		// キーボードの状態変化時
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

		// マウスの状態変化時
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
