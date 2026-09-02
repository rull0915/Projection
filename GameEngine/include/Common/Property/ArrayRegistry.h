//====================================================//
// ファイル名   : ArrayRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/02
//
// 概要 : プロパティが配列を扱えるようにするための補助クラス
//
// 更新履歴 :
// 2026/09/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <typeindex>
#include "Property.h"
#include "Common/CheckVector.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	class ArrayRegistry
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------
	private:

		// 配列のサイズを取得する関数マップ
		std::unordered_map<std::type_index, std::function<size_t(const void*)>> m_getArraySizeMap;

		// 配列の要素を取得する関数マップ
		std::unordered_map<std::type_index, std::function<void*(void*, size_t)>> m_getArrayElementMap;

		// 配列の要素を追加する関数マップ
		std::unordered_map<std::type_index, std::function<void(void*)>> m_addElementMap;

		// 配列の要素を削除する関数マップ
		std::unordered_map<std::type_index, std::function<void(void*, size_t)>> m_removeElementMap;
		
		// 型のプロパティタイプマップ
		std::unordered_map<std::type_index, PropertyType> m_propertyTypeMap;

		// 配列が持つ型のtype_indexの対応マップ
		std::unordered_map<std::type_index, std::type_index> m_typeMap;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ArrayRegistry() = default;
		~ArrayRegistry() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// シングルトン化
		static ArrayRegistry& Instance()
		{
			static ArrayRegistry instance;
			return instance;
		}

		/// <summary>
		/// 型の登録を行う関数
		/// </summary>
		/// <typeparam name="EnumType">登録する列挙型</typeparam>
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
			m_removeElementMap[idx] =
				[](void* p, size_t index)
				{
					// 配列に変換
					ArrayType& vec = *static_cast<ArrayType*>(p);

					// 範囲内なら
					if (index < vec.size())
						// 削除
						vec.erase(vec.begin() + index);				
				};

			// プロパティタイプを持つマップ
			m_propertyTypeMap[idx] = type;

			// 管理対象の型を持つマップ
			m_typeMap.emplace(idx, std::type_index(typeid(ArrayType::value_type)));
		}

		// 配列サイズを返す関数
		size_t GetSize(std::type_index idx, const void* p) const { return m_getArraySizeMap.at(idx)(p); }

		// 配列の要素を取得する関数
		void* GetElement(std::type_index idx, void* p, size_t index) const { return m_getArrayElementMap.at(idx)(p, index); }

		// 配列に要素を追加する関数
		void AddElement(std::type_index idx, void* p) { m_addElementMap.at(idx)(p); }

		// 配列から要素を削除する関数
		void RemoveElement(std::type_index idx, void* p, size_t index) { m_removeElementMap.at(idx)(p, index); }

		// タイプを取得する関数
		PropertyType GetPropertyType(std::type_index idx) { return m_propertyTypeMap.at(idx); }

		// 管理している型を取得する関数
		std::type_index GetVectorType(std::type_index idx) { return m_typeMap.at(idx); }

		// 管理している型を取得する関数
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
}	// namespace REngine
