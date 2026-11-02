/**
 * @file    WinProc.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 *
 * @brief   ウィンドウプロシージャ関数を定義したヘッダ
 */

#include <Windows.h>

namespace RHEngine
{
	/**
	 * @brief ウィンドウプロシージャ
	 */
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
}
