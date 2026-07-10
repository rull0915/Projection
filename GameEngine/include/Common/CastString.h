//====================================================//
// ファイル名   : CastString.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/10
//
// 概要 : 文字列の変換をする関数群
//
// 更新履歴 :
// 2026/07/10 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include <Windows.h>

//====================================================//
// クラス宣言
//====================================================//

namespace CastString
{
	std::wstring Utf8ToWide(const std::string& str, UINT codePage = CP_UTF8)
	{
		// 空文字列なら空文字列を返す
		if (str.empty()) return {};

		// 必要なバッファサイズを取得
		int size = MultiByteToWideChar(codePage, 0,	str.c_str(), -1, nullptr, 0);

		// 結果を格納するワイド文字列を用意
		std::wstring result(size - 1, L'\0');

		// 変換
		MultiByteToWideChar(codePage, 0, str.c_str(), -1, result.data(), size);

		// 返す
		return result;
	}

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
