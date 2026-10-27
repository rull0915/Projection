/**
 * @file    EnumRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/21
 *
 * @brief   プロパティが列挙型を扱えるようにするための補助クラス
 */

#pragma once

#include <typeindex>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <type_traits>

#include "ThirdParty/magic_enum/magic_enum.hpp"

namespace RHEngine
{
	/**
	 * @brief プロパティとして列挙型を扱えるようにするためのクラス
	 */
	class EnumRegistry
	{
	private:

		/** @brief 列挙子名の配列を取得する関数マップ */
		std::unordered_map<std::type_index, std::function<std::vector<std::string>()>> m_getNamesFuncMap;

		/** @brief 列挙型変数から列挙子名を取得する関数マップ */
		std::unordered_map<std::type_index, std::function<std::string(const void*)>> m_getCurrentNameFuncMap;

		/** @brief 列挙子名から実際の変数の値をセットする関数マップ */
		std::unordered_map<std::type_index, std::function<bool(void*, const std::string&)>> m_setByNameFuncMap;

	public:

		/// @name ライフサイクル
		/// @{

		EnumRegistry() = default;

		~EnumRegistry() = default;

		/// @}

		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static EnumRegistry& Instance()
		{
			static EnumRegistry instance;
			return instance;
		}

		/**
		 * @brief 列挙型を登録する
		 * @tparam EnumType 列挙型
		 */
		template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
		void Register()
		{
			// タイプインデックスを取得
			auto idx = std::type_index(typeid(EnumType));

			// 既に登録されている型なら何もしない
			if (m_getNamesFuncMap.contains(idx)) return;

			// 列挙子名を全取得する関数の追加
			m_getNamesFuncMap[idx] =
				[]() -> std::vector<std::string>
				{
					// 結果を格納する配列
					std::vector<std::string> result;

					// enum_magicを使用しリストを取得
					for (auto& n : magic_enum::enum_names<EnumType>()) result.emplace_back(n);

					// 結果を返す
					return result;
				};

			// 変数から列挙子名を取得する関数の追加
			m_getCurrentNameFuncMap[idx] =
				[](const void* value)->std::string
				{
					// 列挙型に変換
					EnumType e = *static_cast<const EnumType*>(value);

					// magic_enumから取得し返す
					return std::string(magic_enum::enum_name(e));
				};

			// 列挙子名から値を変更する関数の追加
			m_setByNameFuncMap[idx] =
				[](void* target, std::string_view name) -> bool
				{
					// 列挙子名から列挙子を取得
					auto opt = magic_enum::enum_cast<EnumType>(name);

					// 変換に失敗したらfalse
					if (!opt) return false;

					// ポインタの指す値を変更
					*static_cast<EnumType*>(target) = *opt;

					// 成功したことを返す
					return true;
				};
		}

		/**
		 * @brief 列挙子名の配列を取得する
		 * @param idx 取得する列挙型のtype_index
		 * @return 列挙子名の配列
		 */
		std::vector<std::string> GetNames(std::type_index idx) const { return m_getNamesFuncMap.at(idx)(); }

		/**
		 * @brief 列挙子名を取得する
		 * @param idx 取得する列挙型のtype_index
		 * @param ptr 列挙型変数のアドレス
		 * @return 列挙子名
		 */
		std::string GetCurrentName(std::type_index idx, const void* ptr) const { return m_getCurrentNameFuncMap.at(idx)(ptr); }

		/**
		 * @brief 列挙子名から値を変更する
		 * @param idx 変更する列挙型のtype_index
		 * @param ptr 変更する列挙型変数のアドレス
		 * @param name 変更後の列挙子名
		 * @return 成功したかどうか
		 */
		bool SetByName(std::type_index idx, void* ptr, const std::string& name) const { return m_setByNameFuncMap.at(idx)(ptr, name); }
	};
}	// namespace RHEngine
