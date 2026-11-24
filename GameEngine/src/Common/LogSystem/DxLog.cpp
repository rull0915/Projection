/**
 * @file    DxLog.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/22
 */

#include "pch.h"
#include "Common/LogSystem/DxLog.h"

#include "Common/CastString.h"

namespace RHEngine
{
	std::string HrToString(HRESULT hr)
	{
		char* msgBuf = nullptr;
		DWORD n = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&msgBuf),
			0,
			nullptr);

		std::string msg = msgBuf ? msgBuf : "Unknown error";

		if (msgBuf)
			LocalFree(msgBuf);

		return msg;
	}

	void FlushD3D11Messages(ID3D11InfoQueue* pQueue)
	{
		if (!pQueue) return;

		// 対象のメッセージの数を取得
		const UINT64 n = pQueue->GetNumStoredMessagesAllowedByRetrievalFilter();

		for (UINT64 i = 0; i < n; ++i)
		{
			// メッセージの長さを取得
			SIZE_T len = 0;
			if (FAILED(pQueue->GetMessageW(i, nullptr, &len))) continue;

			// メッセージの長さに応じてバッファを確保
			std::vector<char> buf(len);
			// バッファを D3D11_MESSAGE にキャスト
			auto* msg = reinterpret_cast<D3D11_MESSAGE*>(buf.data());

			// メッセージを取得
			if (FAILED(pQueue->GetMessageW(i, msg, &len))) continue;

			// ログレベルを決定
			LogLevel lv = LogLevel::Info;
			switch (msg->Severity)
			{
			case D3D11_MESSAGE_SEVERITY_CORRUPTION:
			case D3D11_MESSAGE_SEVERITY_ERROR:   lv = LogLevel::Error;   break;
			case D3D11_MESSAGE_SEVERITY_WARNING: lv = LogLevel::Warning; break;
			default: break;
			}

			// ログ出力
			Logger::Instance().Log(lv, std::source_location::current(), "[D3D11] {}", msg->pDescription);
		}

		// すべてのメッセージをクリア
		pQueue->ClearStoredMessages();
	}

	bool CheckHr(HRESULT hr, const char* expr, LogLevel level, std::source_location loc)
	{
		// 成功した場合は何もしない
		if (SUCCEEDED(hr)) return true;

		// 失敗した場合はログ出力
		auto& logger = Logger::Instance();
		if (level >= logger.GetMinLevel())
		{
			logger.Log(level, loc, "{} failed: {} (0x{:08X})",
				expr, HrToString(hr), static_cast<uint32_t>(hr));
		}

		return false;
	}
}
