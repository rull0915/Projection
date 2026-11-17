/**
 * @file    Logs.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   ログ発行用のマクロをまとめたヘッダ
 * @details source_location.current()を呼び出し時点での情報にしたいためログはinline関数ではなくマクロで扱う
 */

#pragma once

#include <intrin.h>
#include <source_location>

#include "Logger.h"

#define RH_LOG(lv, cat, ...)													\
	do{																			\
		auto& logger = RHEngine::Logger::Instance();							\
		if (lv >= logger.GetMinLevel())											\
		{																		\
			logger.Log(lv, #cat, std::source_location::current(), __VA_ARGS__);	\
		}																		\
	} while(0)

#ifdef NDEBUG
  #define LOG_TRACE(cat, ...) ((void)0)
  #define LOG_DEBUG(cat, ...) ((void)0)
#else
  #define LOG_TRACE(cat, ...) RH_LOG(RHEngine::LogLevel::Trace, cat, __VA_ARGS__)
  #define LOG_DEBUG(cat, ...) RH_LOG(RHEngine::LogLevel::Debug, cat, __VA_ARGS__)
#endif
#define LOG_INFO(cat, ...)  RH_LOG(RHEngine::LogLevel::Info,    cat, __VA_ARGS__)
#define LOG_WARN(cat, ...)  RH_LOG(RHEngine::LogLevel::Warning, cat, __VA_ARGS__)
#define LOG_ERROR(cat, ...) RH_LOG(RHEngine::LogLevel::Error,   cat, __VA_ARGS__)
#define LOG_FATAL(cat, ...) do { RH_LOG(RHEngine::LogLevel::Fatal, cat, __VA_ARGS__); \
                                 RHEngine::Logger::Flush(); __debugbreak(); } while(0)
