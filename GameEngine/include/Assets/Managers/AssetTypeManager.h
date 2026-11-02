/**
 * @file    AssetTypeManager名.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   アセットタイプの管理クラス
 */

#pragma once

#include <filesystem>
#include <typeindex>

namespace RHEngine
{
	/**
	 * @brief アセットのタイプを管理する
	 */
	class AssetTypeManager
	{
	private:
		/** @brief 拡張子名とアセット名の対応マップ */
		std::unordered_map<std::wstring, std::string> m_assetTypeMap;

		/** @brief 拡張子名とtype_indexの対応マップ */
		std::unordered_map<std::wstring, std::type_index> m_assetClassMap;

	public:
		/// @name ライフサイクル
		/// @{

		AssetTypeManager() = default;

		~AssetTypeManager() = default;

		/// @}

		/**
		 * @brief アセットタイプを登録する
		 * @param extention 拡張子名
		 * @param typeName 対応するタイプ名
		 */
		void RegisterAssetType(const std::wstring& extention, const std::string& typeName);

		/**
		 * @brief 型と拡張子の対応関係を登録する
		 * @tparam T アセット型
		 * @param extention 対応する拡張子
		 */
		template<typename T>
		void RegisterAssetClass(const std::wstring& extention);

		/**
		 * @brief パスから対応するアセット名を取得する
		 * @param path 調べるファイルパス 
		 * @return 対応するアセット名 (未対応の場合空文字列)
		 */
		std::string GetAssetType(const std::filesystem::path& path) const;

		/**
		 * @brief パスから対応するtype_indexを取得する
		 * @param type 調べるファイルパス
		 * @return 対応するtype_index (未対応の場合voidのtype_index)
		 */
		std::type_index GetAssetClass(const std::filesystem::path& path) const;

		/**
		 * @brief アセット名から対応するtype_indexを取得する
		 * @param type 調べるアセット名
		 * @return 対応するtype_index (未対応の場合voidのtype_index)
		 */
		std::type_index GetAssetClassFromType(const std::string& type) const;

		/**
		 * @brief アセット名から対応する拡張子を取得する
		 * @param type 調べるアセット名
		 * @return 対応する拡張子 (未対応の場合空文字列)
		 */
		std::wstring GetExtention(const std::string& type) const;

		/// @}
	};

	template<typename T>
	inline void AssetTypeManager::RegisterAssetClass(const std::wstring& extention)
	{
		// マップに追加
		m_assetClassMap.insert_or_assign(extention, std::type_index(typeid(T)));
	}
}	// namespace RHEngine
