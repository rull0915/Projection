/**
 * @file    LogWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   ログウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

#include "Common/LogSystem/ImGuiLogSink.h"

namespace RHEngine
{
	class LogWindow
	{
	private:
		/** @brief ログシンクの生ポインタ */
		ImGuiLogSink* m_pSink;

		//ImGuiTextFilter               m_textFilter;
		//std::map<std::string, bool>   m_categories;      // <map> が必要
		std::vector<size_t>           m_visible;
		bool m_levelEnabled[6] = { true, true, true, true, true, true };
		bool m_autoScroll = true;

	public:

		/// @name ライフサイクル
		/// @{

		LogWindow();

		/// @}

		/**
		 * @brief ログウィンドウ表示
		 */
		void Draw();
	};
}

#endif
