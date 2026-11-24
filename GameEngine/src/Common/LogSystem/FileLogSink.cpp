/**
 * @file    FileLogSink.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/18
 * @TODO 設定などから出力先フォルダを変更できるようにする
 */

#include "pch.h"
#include "Common/LogSystem/FileLogSink.h"

namespace RHEngine
{
	FileLogSink::FileLogSink()
	{
		namespace fs = std::filesystem;

		// ログ保存ディレクトリのパス
		const fs::path logDir = "Logs";

		// ログディレクトリを作成
		fs::create_directories(logDir);

		// 現在時刻を取得
		const auto now = std::chrono::system_clock::now();
		const auto time = std::chrono::system_clock::to_time_t(now);

		std::tm localTime{};
		localtime_s(&localTime, &time);

		// ファイル名を生成
		const std::string filename = std::format(
			"{:04}-{:02}-{:02}_{:02}-{:02}-{:02}.log",
			localTime.tm_year + 1900,
			localTime.tm_mon + 1,
			localTime.tm_mday,
			localTime.tm_hour,
			localTime.tm_min,
			localTime.tm_sec
		);

		// ファイルを作成
		m_file.open(logDir / filename, std::ios::out);
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
