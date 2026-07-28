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
#include <typeindex>

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

		// 拡張子をキーとしたtype_indexのマップ
		std::unordered_map<std::wstring, std::type_index> m_assetClassMap;

		//------------------------------------
		// 関数宣言
		//------------------------------------

	public:

		// アセット名の登録関数
		void RegisterAssetType(const std::wstring& extention, const std::string& typeName);

		// 拡張子に対応するアセットクラスを登録する関数
		template<typename T>
		void RegisterAssetClass(const std::wstring& extention);

		// 特定のファイルに対応するアセットタイプを取得する関数
		std::string GetAssetType(const std::filesystem::path& path) const;

		// 特定のファイルに対応するtype_indexを取得する関数
		std::type_index GetAssetClass(const std::filesystem::path& path) const;
	};

	template<typename T>
	inline void AssetTypeManager::RegisterAssetClass(const std::wstring& extention)
	{
		// マップに追加
		m_assetClassMap.insert_or_assign(extention, std::type_index(typeid(T)));
	}
}	// namespace REngine
