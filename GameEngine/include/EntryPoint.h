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

// #include "Engine/Application.h"
#include "System/WindowManager.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <GamePad.h>

#ifdef ENGINE_GUI
#include "Editor/Editor/ImguiManager.h"
#endif

//====================================================//
// 前方宣言
//====================================================//
class Game;

//====================================================//
// インクルードファイル
//====================================================//

// Windows API
#include <Windows.h>

// 標準ライブラリ
#include <iterator>  // std::ignore

// DirectXTK Input
#include <KeyBoard.h>
#include <Mouse.h>
#include <GamePad.h>

//====================================================//
// 関数宣言
//====================================================//

// Windows Procedure 前方宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// エントリポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

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
}
