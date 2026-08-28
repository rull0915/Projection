//====================================================//
// ファイル名   : PropertyObject.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/02
//
// 概要 : プロパティをまとめておくクラス
//
// 更新履歴 :
// 2026/07/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>
#include <string>
#include <vector>

#include "Property.h"
#include "Assets/Objects/Handle.h"
#include "Common/ObjectReference.h"
#include "EnumRegistry.h"
#include "AssetPropertyRegistry.h"

// マクロ
#define ADD_PROPERTY(property) (AddProperty(#property, &property))

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	// プロパティを扱うオブジェクトの基底クラス
	class PropertyObject
	{
	private:
		// プロパティ一覧
		std::vector<Property> m_properties;

	protected:
		// コンストラクタ
		PropertyObject()
			: m_properties{}
		{}
		// コピー禁止
		PropertyObject(const PropertyObject&) = delete;
		PropertyObject& operator=(const PropertyObject&) = delete;

		// デストラクタ
		virtual ~PropertyObject() = default;

	public:
		// プロパティを取得する関数
		const std::vector<Property>& GetPropaties() const { return m_properties; }

	protected:
		// プロパティの追加関数
		template<typename T>
		void AddProperty(std::string name, T* value)
		{
			Property prop{ name, GetPropertyType<T>(), value };

			// 列挙型なら
			if constexpr (std::is_enum_v<T>)
			{
				// 登録
				EnumRegistry::Instance().Register<T>();

				// タイプインデックスを保存
				prop.typeIndex = std::type_index(typeid(T));
			}
			// AssetHandleなら
			else if constexpr (IsHandle_v<T>)
			{
				// 登録
				AssetPropertyRegistry::Instance().Register<typename T::value_type>();

				// タイプインデックスを保存
				prop.typeIndex = std::type_index(typeid(typename T::value_type));
			}
			// ObjectRefなら
			else if constexpr (IsRef_v<T>)
			{
				// 登録
				AssetPropertyRegistry::Instance().Register<typename T::value_type>();

				// タイプインデックスを保存
				prop.typeIndex = std::type_index(typeid(typename T::value_type));
			}

			// 配列に追加
			m_properties.push_back(prop);
		}

	private:
		// 型からタイプを取得する関数
		template<typename T>
		PropertyType GetPropertyType()
		{
			// bool
			if constexpr (std::is_same_v<T, bool>) return PropertyType::Bool;
			// int
			else if constexpr (std::is_same_v<T, int>) return PropertyType::Int;
			// float
			else if constexpr (std::is_same_v<T, float>) return PropertyType::Float;
			// string
			else if constexpr (std::is_same_v<T, std::string>) return PropertyType::String;
			// Vector2
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector2>) return PropertyType::Vector2;
			// Vector3
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector3>) return PropertyType::Vector3;
			// Quaternion
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Quaternion>) return PropertyType::Quaternion;
			// Color
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Color>) return PropertyType::Color;
			// PropertyObject派生
			else if constexpr (std::is_base_of_v<PropertyObject, T>) return PropertyType::Object;
			// 列挙型
			else if constexpr (std::is_enum_v<T>) return PropertyType::Enum;
			// Handle
			else if constexpr (IsHandle_v<T>) return PropertyType::AssetHandle;
			// Ref
			else if constexpr (IsRef_v<T>) return PropertyType::ObjectRef;

			// その他
			else return PropertyType::None;
		}
	};
} // namespace REngine
