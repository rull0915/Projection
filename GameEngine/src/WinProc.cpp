//====================================================//
// ファイル名  : WinProc.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/21
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "WinProc.h"

#include <Mouse.h>
#include <Keyboard.h>

#include "Editor/Editor/ImguiManager.h"
#include "Application.h"

//====================================================//
// 関数の実体宣言
//====================================================//

LRESULT CALLBACK RHEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

#ifdef ENGINE_GUI
	if (RHEngine::ImguiManager::ProcessMessage(hWnd, message, wParam, lParam))
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

	case WM_SIZE:
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		if (width == 0 || height == 0) break;

		// app->OnWindowSizeChanged(width, height);

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
