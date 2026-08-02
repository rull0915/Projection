//====================================================//
// ファイル名  : AssetTypeManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要       : アセットタイプの管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Managers/AssetTypeManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void AssetTypeManager::RegisterAssetType(const std::wstring& extention, const std::string& typeName)
	{
		// 存在するキーなら何もしない
		if (m_assetTypeMap.find(extention) != m_assetTypeMap.end()) return;

		// マップに追加
		m_assetTypeMap.insert({ extention, typeName });
	}

	std::string AssetTypeManager::GetAssetType(const std::filesystem::path& path) const
	{
		// 拡張子を取得
		auto ext = path.extension().wstring();

		// 大文字小文字を無視する
		std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

		// 存在するキーなら
		if (auto it = m_assetTypeMap.find(ext); it != m_assetTypeMap.end())
		{
			// 対応するタイプ文字列を返す
			return it->second;
		}

		// 存在しないキーなら空文字列を返す
		return "";
	}

	std::type_index AssetTypeManager::GetAssetClass(const std::filesystem::path& path) const
	{
		// 拡張子を取得
		auto ext = path.extension().wstring();

		// 大文字小文字を無視する
		std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

		// 存在するキーなら
		if (auto it = m_assetClassMap.find(ext); it != m_assetClassMap.end())
		{
			// 対応するタイプ文字列を返す
			return it->second;
		}

		// 存在しないキーならvoidとしておく
		return std::type_index(typeid(void));
	}

	std::wstring AssetTypeManager::GetExtention(const std::string& type) const
	{
		// ループ
		for (auto& map : m_assetTypeMap)
		{
			// 一致すれば返す
			if (map.second == type) return map.first;
		}

		// なければ空文字を返す
		return L"";
	}
}	// namespace REngine
