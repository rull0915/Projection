/**
 * @file    EntryPoint.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 *
 * @brief   EntryPointを定義するヘッダ
 * @details エンジン側にエントリポイントを置くことでゲームを作るたびにウィンドウの作成処理などを書く手間を減らす
 */

// Windows API
#include <Windows.h>

#include <DirectXMath.h>

// 標準ライブラリ
#include <memory>

// DirectXTK Input
#include <KeyBoard.h>
#include <GamePad.h>
#include <Mouse.h>

// RHEngine
#include "Application.h"
#include "WinProc.h"

// 外部で定義されたCreateApplicationを呼び出す
extern RHEngine::Application* RHEngine::CreateApplication();

namespace {
	std::unique_ptr<RHEngine::Application> g_app;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// 使用しないパラメータ
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!DirectX::XMVerifyCPUSupport()) return 1;
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	if (FAILED(hr)) return 1;

	// アプリケーションインスタンスを生成
	g_app.reset(RHEngine::CreateApplication());
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
		wcex.lpfnWndProc = RHEngine::WndProc;
		wcex.hInstance = hInstance;
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.lpszClassName = L"RHEngine_Window";
		if (!RegisterClassExW(&wcex)) return 1;

		// ウィンドウサイズの指定
		RECT rc = { 0, 0, static_cast<LONG>(props.width), static_cast<LONG>(props.height) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		// ウィンドウの作成
		HWND hwnd = CreateWindowExW(0, L"RHEngine_Window", props.title.c_str(), WS_OVERLAPPEDWINDOW,
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
	g_app->Finalize();

	// アプリケーションのリセット
	g_app.reset();

	return static_cast<int>(msg.wParam);
}
