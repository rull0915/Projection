//====================================================//
// ファイル名   : ResourceReader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要 : リソースの読み込みを自動化するクラス
//
// 更新履歴 :
// 2026/07/08 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <filesystem>

//====================================================//
// クラス宣言
//====================================================//
class ResourceReader
{
public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ResourceReader() = default;
    ~ResourceReader() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

	// モデル
	static void ReadModels(const std::wstring& path);

	// テクスチャ
	static void ReadTextures(const std::wstring& path);

	// 音
	static void ReadSounds(const std::wstring& path);

	// フォント
	static void ReadFonts(const std::wstring& path);

	// オブジェクト
	static void ReadObjects(const std::wstring& path);

private:

	static std::vector<std::filesystem::path> Read(const std::wstring& path, const std::vector<std::string>& extentions);
};
