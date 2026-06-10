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
#include "WindowManager.h"

#include "Game/Game.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void WindowManager::SwitchScreenMode(HWND hWnd, Game* game)
{
    static bool s_fullscreen = false;

    if (s_fullscreen)
    {
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_MYSTYLE);
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

        int width = Screen::WIDTH;
        int height = Screen::HEIGHT;

        if (game)
            game->GetDefaultSize(width, height);

        ShowWindow(hWnd, SW_SHOWNORMAL);

        RECT rc = { 0,0,width,height };
        AdjustWindowRect(&rc, WS_MYSTYLE, FALSE);

        SetWindowPos(
            hWnd,
            HWND_TOP,
            0, 0,
            rc.right - rc.left,
            rc.bottom - rc.top,
            SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
        );
    }
    else
    {
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

        SetWindowPos(
            hWnd,
            HWND_TOP,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
        );

        ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    }

    s_fullscreen = !s_fullscreen;
}
