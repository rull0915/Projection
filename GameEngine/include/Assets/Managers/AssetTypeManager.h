//====================================================//
// ファイル名   : AssetTypeManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : アセットタイプの管理クラス
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <filesystem>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetTypeManager
	{
		//------------------------------------
		// メンバ変数
		//------------------------------------

	private:
		// 拡張子をキーとしたAssetTypeのマップ
		std::unordered_map<std::wstring, std::string> m_assetTypeMap;

		//------------------------------------
		// 関数宣言
		//------------------------------------

	public:

		// アセット名の登録関数
		void RegisterAssetType(const std::wstring& extention, const std::string& typeName);

		// 特定のファイルに対応するアセットタイプを取得する関数
		std::string GetAssetType(const std::filesystem::path& path) const;
	};
}	// namespace REngine
