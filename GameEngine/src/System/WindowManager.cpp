//====================================================//
// ファイル名  : WindowManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/10
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void WindowManager::SwitchScreenMode()
	{
		if (!m_window) return;

		static bool s_fullscreen = false;

		if (s_fullscreen)
		{
			SetWindowLongPtr(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowLongPtr(m_window, GWL_EXSTYLE, 0);

			ShowWindow(m_window, SW_SHOWNORMAL);

			RECT rc = { 0,0,static_cast<LONG>(m_defaultWidth),static_cast<LONG>(m_defaultHeight) };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

			SetWindowPos(
				m_window,
				HWND_TOP,
				0, 0,
				rc.right - rc.left,
				rc.bottom - rc.top,
				SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
			);
		}
		else
		{
			SetWindowLongPtr(m_window, GWL_STYLE, WS_POPUP);
			SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_TOPMOST);

			SetWindowPos(
				m_window,
				HWND_TOP,
				0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
			);

			ShowWindow(m_window, SW_SHOWMAXIMIZED);
		}

		s_fullscreen = !s_fullscreen;
	}
}	// namespace RHEngine
