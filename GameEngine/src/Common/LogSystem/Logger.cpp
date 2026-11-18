/**
 * @file    Logger.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 */

#include "pch.h"
#include "Common/LogSystem/Logger.h"

#include <filesystem>

namespace RHEngine
{
	void Logger::ClearSinks()
	{
		std::lock_guard lk(m_mutex);

		m_sinks.clear();
	}

	void Logger::Flush()
	{
		std::lock_guard lk(m_mutex);

		for (auto& sink : m_sinks)
		{
			sink->Flush();
		}
	}

	void Logger::Dispatch(LogLevel lv, std::source_location loc, std::string msg)
	{
		// エントリを作成
		LogEntry e{};
		e.level = lv;
		e.message = std::move(msg);
		e.line = loc.line();

		// ファイル名のみを抽出
		e.file = std::filesystem::path(loc.file_name()).filename().string();

		// atomicから数値を読み取る
		e.frame = m_frameCount.load(std::memory_order_relaxed);

		// 現在実行中のスレッドのIDを取得する
		e.threadId = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

		// 現在の時刻を取得する
		e.time = std::format("{:%Y-%m-%d %H:%M:%S}",
			std::chrono::zoned_time{ std::chrono::current_zone(), 
				floor<std::chrono::milliseconds>(std::chrono::system_clock::now()) });

		// Sinkを保護
		std::lock_guard lk(m_mutex);

		// 全Sinkで書き込み
		for (auto& s : m_sinks) s->Write(e);
	
		// Error以上の重大なログの場合は即時Flush
		if (lv >= LogLevel::Error) for (auto& s : m_sinks) s->Flush();
	}
}
