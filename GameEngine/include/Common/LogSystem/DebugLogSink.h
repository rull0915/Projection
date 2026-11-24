/**
 * @file    DebugLogSink.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   OutputDebugStringで扱うログシンク
 */

#pragma once

#include <string>
#include <format>
#include <Windows.h>

#include "Log.h"
#include "Common/CastString.h"

namespace RHEngine
{
	class DebugLogSink : public ILogSink
	{
	public:

		/// @name ライフサイクル
		/// @{
		
		explicit DebugLogSink() 
		{}

		/// @}

		/**
		 * @brief エントリを増やす
		 * @param e 対象のエントリ
		 */
		void Write(const LogEntry& e) override
		{
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
			std::wstring wResult = CastString::Utf8ToWide(result);
			OutputDebugStringW(wResult.c_str());
		}
	};
}
