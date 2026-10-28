/**
 * @file    WinProc.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 */

#include "pch.h"
#include "WinProc.h"

#include <Mouse.h>
#include <Keyboard.h>

#include "Editor/Editor/ImguiManager.h"
#include "Application.h"

namespace RHEngine
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static bool s_in_sizemove = false;
		static bool s_in_suspend = false;
		static bool s_minimized = false;
		static bool s_fullscreen = false;

		auto app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

#ifdef ENGINE_GUI
		if (ImguiManager::ProcessMessage(hWnd, message, wParam, lParam))
		{
			return true;
		}
#endif // USE_IMGUi

		// 受け取ったメッセージによって処理を分岐
		switch (message)
		{
		case WM_CREATE:

			if (lParam)
			{
				auto params = reinterpret_cast<LPCREATESTRUCTW>(lParam);

				SetWindowLongPtr(
					hWnd,
					GWLP_USERDATA,
					reinterpret_cast<LONG_PTR>(params->lpCreateParams)
				);
			}

			break;

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
			if (wParam == SIZE_MINIMIZED)
			{
				if (!s_minimized)
				{
					s_minimized = true;
					if (!s_in_suspend && app)
						//app->OnSuspending();
						s_in_suspend = true;
				}
			}
			else if (s_minimized)
			{
				s_minimized = false;
				if (s_in_suspend && app)
					//app->OnResuming();
					s_in_suspend = false;
			}
			else if (!s_in_sizemove && app)
			{
				app->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 200l;
			info->ptMinTrackSize.y = 200l;
		}
		break;

		case WM_MOVE:
			if (app)
			{
				// app->OnWindowMoved();
			}
			break;
		case WM_ENTERSIZEMOVE:
			s_in_sizemove = true;
			break;

		case WM_EXITSIZEMOVE:
			s_in_sizemove = false;
			if (app)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);

				app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}	// namespace RHEngine
