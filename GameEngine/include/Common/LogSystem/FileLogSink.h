/**
 * @file    FileLogSink.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   外部ファイルへ出力するログシンク
 */

#pragma once

#include <filesystem>
#include <fstream>

#include "Log.h"

namespace RHEngine
{
	class FileLogSink : public ILogSink
	{
	private:
		
		/** @brief 編集対象のファイル */
		std::ofstream m_file;

	public:

		/// @name ライフサイクル
		/// @{
		
		explicit FileLogSink();

		~FileLogSink();

		/// @}

		/**
		 * @brief エントリを増やす
		 * @param e 対象のエントリ
		 */
		void Write(const LogEntry& e) override;

		/**
		 * @brief ofsのflushを呼ぶ
		 */
		void Flush() override;
	};
}	// namespace RHEngine
