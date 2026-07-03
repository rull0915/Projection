//====================================================//
// ファイル名   : GetExtension.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/03
//
// 概要 : 拡張子を取得するヘッダ
//
// 更新履歴 :
// 2026/07/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include <filesystem>

//====================================================//
// クラス宣言
//====================================================//
namespace GetExtension
{
	static const std::string Get(const std::wstring& filePath)
	{
		// ファイルパスを解析
		std::filesystem::path path(filePath);

		// 拡張子を取得
		std::string extension = path.extension().string();

		// 安全のために小文字に変換
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		// 拡張子を返す
		return extension;
	}
};
