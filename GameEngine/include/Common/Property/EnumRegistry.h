//====================================================//
// ファイル名   : EnumRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : プロパティが列挙型を扱えるようにするための補助クラス
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>

#include "ThirdParty/magic_enum/magic_enum.hpp"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	class EnumRegistry
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------
	private:

		// 列挙子の名前の配列を取得する関数マップ
		std::unordered_map<std::type_index, std::function<std::vector<std::string>()>> m_getNamesFuncMap;

		// 実際の変数から列挙子名を取得する関数マップ
		std::unordered_map<std::type_index, std::function<std::string(const void*)>> m_getCurrentNameFuncMap;

		// 列挙子名から実際の変数の値をセットする関数マップ
		std::unordered_map<std::type_index, std::function<bool(void*, std::string_view)>> m_setByNameFuncMap;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		EnumRegistry() = default;
		~EnumRegistry() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// シングルトン化
		static EnumRegistry& Instance()
		{
			static EnumRegistry instance;
			return instance;
		}

		/// <summary>
		/// 型の登録を行う関数
		/// </summary>
		/// <typeparam name="EnumType">登録する列挙型</typeparam>
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

		// 列挙子名の配列を返す関数
		std::vector<std::string> GetNames(std::type_index idx) const { return m_getNamesFuncMap.at(idx)(); }

		// 指定したポインタの列挙子名を返す関数
		std::string GetCurrentName(std::type_index idx, const void* ptr) const { return m_getCurrentNameFuncMap.at(idx)(ptr); }

		// 列挙子名から値を変更する関数
		bool SetByName(std::type_index idx, void* ptr, std::string_view name) const { return m_setByNameFuncMap.at(idx)(ptr, name); }
	};
}	// namespace REngine
