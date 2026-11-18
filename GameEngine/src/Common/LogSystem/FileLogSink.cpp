/**
 * @file    FileLogSink.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/18
 */

#include "pch.h"
#include "Common/LogSystem/FileLogSink.h"

namespace RHEngine
{
	FileLogSink::FileLogSink(const std::filesystem::path& path)
	{
		// 追記モードでファイルを開く
		m_file.open(path, std::ios::out | std::ios::app);
	}

	FileLogSink::~FileLogSink()
	{
		Flush();
	}

	void FileLogSink::Write(const LogEntry& e)
	{
		if (!m_file.is_open())
			return;

		std::string result =
			std::format(
				"[{}] [{}] [{}:{}] [Frame:{}] [Thread:{}] {}\n",
				ToString(e.level),
				e.time,
				e.file,
				e.line,
				e.frame,
				e.threadId,
				e.message
			);

		// デバッグウィンドウへ出力
		m_file << result;
	}

	void FileLogSink::Flush()
	{
		if (m_file.is_open())
			m_file.flush();
	}
}
