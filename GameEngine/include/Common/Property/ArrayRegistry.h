/**
 * @file    ArrayRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/02
 *
 * @brief   プロパティが配列を扱えるようにするための補助クラス
 */

#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include "Property.h"
#include "PropertyTypeIndex.h"

namespace RHEngine
{
	/**
	 * @brief 配列に関する情報を格納するレジストリ
	 */
	class ArrayRegistry
	{
	private:

		/** @brief 配列のサイズを取得する関数マップ */
		std::unordered_map<std::type_index, std::function<size_t(const void*)>> m_getArraySizeMap;

		/** @brief 配列をリサイズする関数マップ */
		std::unordered_map<std::type_index, std::function<void(void*, size_t)>> m_changeArraySizeMap;

		/** @brief 配列の要素を取得する関数マップ */
		std::unordered_map<std::type_index, std::function<void*(void*, size_t)>> m_getArrayElementMap;

		/** @brief 配列の要素を追加する関数マップ */
		std::unordered_map<std::type_index, std::function<void(void*)>> m_addElementMap;

		/** @brief 配列の末尾を削除する関数マップ */
		std::unordered_map<std::type_index, std::function<void(void*)>> m_popBackElementMap;
		
		/** @brief 管理している型のプロパティタイプを取得する関数マップ */
		std::unordered_map<std::type_index, PropertyType> m_propertyTypeMap;

		/** @brief 管理している型のプロパティタイプインデックスを取得するマップ */
		std::unordered_map<std::type_index, std::type_index> m_typeMap;

	public:

		/// @name ライフサイクル
		/// @{

		ArrayRegistry() = default;

		~ArrayRegistry() = default;

		/// @}

		/**
		 * @brief インスタンスを取得する (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static ArrayRegistry& Instance()
		{
			static ArrayRegistry instance;
			return instance;
		}

		/**
		 * @brief 型の登録を行う
		 * @tparam 配列の型
		 * @param 管理する型に対応するプロパティタイプ
		 */
		template<typename ArrayType>
		void Register(PropertyType type)
		{
			// タイプインデックスを取得
			auto idx = std::type_index(typeid(ArrayType));

			// 既に登録されている型なら何もしない
			if (m_getArraySizeMap.contains(idx)) return;

			// 配列サイズを取得する関数の登録
			m_getArraySizeMap[idx] =
				[](const void* p) -> size_t
				{
					// 配列に変換
					return static_cast<const ArrayType*>(p)->size();
				};

			// 配列サイズを変更する関数の登録
			m_changeArraySizeMap[idx] =
				[](void* p, size_t size)
				{
					// 配列に変換
					static_cast<ArrayType*>(p)->resize(size);
				};

			// 配列の要素を取得する関数の登録
			m_getArrayElementMap[idx] =
				[](void* p, size_t index) -> void*
				{
					// 配列に変換
					ArrayType* vec = static_cast<ArrayType*>(p);

					// 範囲内なら
					if (index < vec->size())
						// 要素を取得し返す
						return static_cast<void*>(&(*vec)[index]);

					// 範囲外ならnull
					return nullptr;
				};

			// 配列に要素を追加する関数の登録
			m_addElementMap[idx] =
				[](void* p)
				{
					static_cast<ArrayType*>(p)->emplace_back();
				};

			// 配列の要素を削除する関数の登録
			m_popBackElementMap[idx] =
				[](void* p)
				{
					// 配列に変換
					ArrayType& vec = *static_cast<ArrayType*>(p);

					// 要素があれば
					if (!vec.empty())
						// 末尾削除
						vec.pop_back();
				};

			// プロパティタイプを持つマップ
			m_propertyTypeMap[idx] = type;

			// 管理対象の型を持つマップ
			m_typeMap.emplace(idx, PropertyTypeIndex::GetTypeIndex<typename ArrayType::value_type>());
		}

		/**
		 * @brief 配列サイズを取得する
		 * @param idx 配列型のtype_index
		 * @param p 取得対象の配列の先頭アドレス
		 * @return 配列のサイズ
		 */
		size_t GetSize(std::type_index idx, const void* p) const { return m_getArraySizeMap.at(idx)(p); }

		/**
		 * @brief 配列をリサイズする関数
		 * @param idx 配列型のtype_index
		 * @param p 取得対象の配列の先頭アドレス
		 * @param size 変更後のサイズ
		 */
		void Resize(std::type_index idx, void* p, size_t size) const { return m_changeArraySizeMap.at(idx)(p, size); }

		/**
		 * @brief 配列の要素を取得する
		 * @param idx 配列型のtype_index
		 * @param p 取得対象の配列の先頭アドレス
		 * @param index 取得するインデックス
		 * @return 要素へのポインタ
		 */
		void* GetElement(std::type_index idx, void* p, size_t index) const { return m_getArrayElementMap.at(idx)(p, index); }

		/**
		 * @brief 配列に要素を追加する
		 * @details 追加された要素はデフォルト値
		 * @param idx 配列型のtype_index
		 * @param p 取得対象の配列の先頭アドレス
		 */
		void AddElement(std::type_index idx, void* p) { m_addElementMap.at(idx)(p); }

		/**
		 * @brief 配列から末尾の要素を削除する
		 * @param idx 配列型のtype_index
		 * @param p 取得対象の配列の先頭アドレス
		 */
		void PopBackElement(std::type_index idx, void* p) { m_popBackElementMap.at(idx)(p); }

		/**
		 * @brief 配列型から管理しているプロパティタイプを取得する
		 * @param idx 配列型のtype_index
		 * @return 管理しているプロパティタイプ
		 */
		PropertyType GetPropertyType(std::type_index idx) { return m_propertyTypeMap.at(idx); }

		/**
		 * @brief 管理しているタイプを取得する
		 * @param idx 配列型のtype_index
		 * @return 管理している型のtype_index
		 */
		std::type_index GetVectorType(std::type_index idx) { return m_typeMap.at(idx); }

		/**
		 * @brief プロパティを生成する
		 * @param idx 配列型のtype_index
		 * @param p 対象の配列の先頭アドレス
		 * @param index プロパティを生成する要素のインデックス
		 * @return 生成したプロパティ
		 */
		Property GetProperty(std::type_index idx, void* p, size_t index) 
		{
			Property pro;
			pro.name = std::string("Element ") + std::to_string(index);
			pro.type = GetPropertyType(idx);
			pro.typeIndex = GetVectorType(idx);
			pro.value = GetElement(idx, p, index);

			return pro;
		}
	};
}	// namespace RHEngine
