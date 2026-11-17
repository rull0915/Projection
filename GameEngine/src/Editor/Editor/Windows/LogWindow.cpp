/**
 * @file    LogWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 */

#include "pch.h"
#include "LogWindow.h"

#ifdef ENGINE_GUI

#include "Common/LogSystem/Logger.h"

namespace RHEngine
{
	LogWindow::LogWindow()
	{
		// シンクを取得
	 	m_pSink = Logger::Instance().AddSink<ImGuiLogSink>();
	}

	void LogWindow::Draw()
	{
		// シンク経由でエントリを取得する
		if (!m_pSink) return;

		m_pSink->Access([](const std::deque<LogEntry>& entries) 
		{
			// 全エントリを走査
			for (auto& entry : entries)
			{

			}
		});
	}
}

#endif // ENGINE_GUI
