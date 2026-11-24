/**
 * @file    LogFatal.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/18
 */

#include "pch.h"
#include "Common/LogSystem/LogFatal.h"

#include <cstdlib>
#include <Windows.h>
#include <intrin.h>

#include "Common/LogSystem/Logger.h"

namespace RHEngine
{
	void FatalExit(const char* msg)
	{
		Logger::Instance().Flush();

		// デバッガの起動中なら
		if (::IsDebuggerPresent())
			::__debugbreak();

		// 致命的なエラーが発生したことを通知する
		::MessageBoxA(nullptr, msg, "RHEngine Fatal Error", MB_OK | MB_ICONERROR);

		// プログラムを異常終了させる
		std::abort();
	}
}	// namespace RHEngine
