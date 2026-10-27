/**
 * @file    CastString.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/10
 *
 * @brief   stringとwstrnigの双変換
 */

#pragma once

#include <string>
#include <Windows.h>

namespace RHEngine
{
	namespace CastString
	{
		/**
		 * @brief utf8文字列からワイド文字列へ変換する
		 * @param str 変換したい文字列
		 * @param codePage 文字コード
		 * @return 変換後のワイド文字列
		 */
		std::wstring Utf8ToWide(const std::string& str, UINT codePage = CP_UTF8)
		{
			// 空文字列なら空文字列を返す
			if (str.empty()) return {};

			// 必要なバッファサイズを取得
			int size = MultiByteToWideChar(codePage, 0, str.c_str(), -1, nullptr, 0);

			// 結果を格納するワイド文字列を用意
			std::wstring result(size - 1, L'\0');

			// 変換
			MultiByteToWideChar(codePage, 0, str.c_str(), -1, result.data(), size);

			// 返す
			return result;
		}

		/**
		 * @brief ワイド文字列からutf8文字列へ変換する
		 * @param str 変換したいワイド文字列
		 * @param codePage 文字コード
		 * @return 変換後のutf8文字列
		 */
		std::string WideToUtf8(const std::wstring& str, UINT codePage = CP_UTF8)
		{
			// 空文字列なら空文字列を返す
			if (str.empty()) return {};

			// 必要なバッファサイズを取得
			int size = WideCharToMultiByte(codePage, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);

			// 結果を格納する文字列を用意
			std::string result(size - 1, '\0');

			// 変換
			WideCharToMultiByte(codePage, 0, str.c_str(), -1, result.data(), size, nullptr, nullptr);

			// 返す
			return result;
		}
	}
} // namespace RHEngine
