/**
 * @file    Logs.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   ログ発行用のマクロをまとめたヘッダ
 * @details source_location.current()を呼び出し時点での情報にしたいためログはinline関数ではなくマクロで扱う
 */

#pragma once

#include <source_location>

#include "Logger.h"
#include "LogFatal.h"

#define RH_LOG(lv, ...)														\
	do{																		\
		auto& logger = RHEngine::Logger::Instance();						\
		if (lv >= logger.GetMinLevel())										\
		{																	\
			logger.Log(lv, std::source_location::current(), __VA_ARGS__);	\
		}																	\
	} while(0)

#ifdef NDEBUG
  #define LOG_TRACE(...) ((void)0)
  #define LOG_DEBUG(...) ((void)0)
#else
  #define LOG_TRACE(...) RH_LOG(RHEngine::LogLevel::Trace, __VA_ARGS__)
  #define LOG_DEBUG(...) RH_LOG(RHEngine::LogLevel::Debug, __VA_ARGS__)
#endif
#define LOG_INFO(...)  RH_LOG(RHEngine::LogLevel::Info,    __VA_ARGS__)
#define LOG_WARN(...)  RH_LOG(RHEngine::LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...) RH_LOG(RHEngine::LogLevel::Error,   __VA_ARGS__)

#define LOG_FATAL(...)	\
	do {				\
		RH_LOG(RHEngine::LogLevel::Fatal, __VA_ARGS__); \
		RHEngine::FatalExit("A fatal error occurred. See the log for details.");							\
	} while(0)
