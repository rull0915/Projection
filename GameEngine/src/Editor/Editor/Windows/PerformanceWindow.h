/**
 * @file    PerformanceWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 *
 * @brief   パフォーマンスを表示するウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

namespace RHEngine
{
	/**
	 * @brief パフォーマンスを表示する
	 */
	class PerformanceWindow
	{
	public:

		/**
		 * @brief パフォーマンスウィンドウを表示する
		 */
		void DrawWindow();
	};
}	// namespace RHEngine

#endif	// ENGINE_GUI
