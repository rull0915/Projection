/**
 * @file    LogWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   ログウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

#include <map>
#include <string>
#include <vector>
#include <SimpleMath.h>

#include "Common/LogSystem/Log.h"
#include "Common/LogSystem/ImGuiLogSink.h"

namespace RHEngine
{
	class LogWindow
	{
	private:
		/** @brief ログシンクの生ポインタ */
		ImGuiLogSink* m_pSink;

		/** @brief 表示されるエントリインデックスの一覧 */
		std::vector<size_t> m_visible;

		/** @brief 各レベルを表示するかどうか */
		bool m_levelEnabled[static_cast<uint8_t>(LogLevel::Count)];

		/** @brief オートスクロールするかどうか */
		bool m_autoScroll;

	public:

		/// @name ライフサイクル
		/// @{

		LogWindow();

		/// @}

		/**
		 * @brief ログレベルごとの色を取得する
		 */
		static DirectX::SimpleMath::Color LevelColor(LogLevel lv)
		{
			switch (lv) {
			case LogLevel::Trace:   return { 0.6f, 0.6f, 0.6f, 1 };
			case LogLevel::Debug:   return { 0.5f, 0.8f, 1.0f, 1 };
			case LogLevel::Info:    return { 1.0f, 1.0f, 1.0f, 1 };
			case LogLevel::Warning: return { 1.0f, 0.85f, 0.2f, 1 };
			case LogLevel::Error:   return { 1.0f, 0.35f, 0.35f, 1 };
			default:                return { 1.0f, 0.1f, 0.6f, 1 };
			}
		}

		/**
		 * @brief ログウィンドウ表示
		 */
		void Draw();
	};
}

#endif
