/**
 * @file    DxLog.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/22
 *
 * @brief   DirectX依存のログ出力用ヘッダ
 */

#pragma once

#include <d3d11.h>
#include <string>
#include <Windows.h>

#include "Logs.h"

namespace RHEngine
{
	/**
	 * @brief HRESULTを文字列にする
	 */
	std::string HrToString(HRESULT hr);

	/**
	 * @brief DirectXのメッセージをログとして出力する
	 */
	void FlushD3D11Messages(ID3D11InfoQueue* pQueue);

	/**
	 * @brief HRESULTの結果を調べログを出力する関数
	 * @param hr 対象のHRESULT
	 * @param expr 説明
	 * @param level ログレベル
	 * @param loc 位置情報
	 * @return 成功したかどうか
	 */
	bool CheckHr(HRESULT hr, const char* expr, LogLevel level, std::source_location loc);
}

#define DX_CHECK(...)															\
	do {																		\
		HRESULT __hr = (__VA_ARGS__);												\
		if (FAILED(__hr))															\
		{																		\
			LOG_FATAL("{} failed: {} (0x{:08X})", #__VA_ARGS__,                 \
                      ::RHEngine::HrToString(__hr), static_cast<uint32_t>(__hr));	\
		}																		\
	} while(0)

// すでに HRESULT 変数がある場合用
#define DX_CHECK_HR(hr, what)													\
	do {																		\
		if (FAILED(hr))															\
		{																		\
			LOG_FATAL("{} failed: {} (0x{:08X})", what,							\
                      ::RHEngine::HrToString(hr), static_cast<uint32_t>(hr));	\
		}																		\
	} while(0)

// Error で出す版
#define DX_TRY(...) \
    ::RHEngine::CheckHr((__VA_ARGS__), #__VA_ARGS__, ::RHEngine::LogLevel::Error, std::source_location::current())

// Warning で出す版
#define DX_TRY_WARN(...) \
    ::RHEngine::CheckHr((__VA_ARGS__), #__VA_ARGS__, ::RHEngine::LogLevel::Warning, std::source_location::current())
