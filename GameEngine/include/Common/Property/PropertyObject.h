//====================================================//
// ファイル名   : PropertyObject.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/02
//
// 概要 : プロパティをまとめておくクラス
//
// 更新履歴 :
// 2026/07/02 新規作成
// 2026/09/04 void*で実アドレスを保持するPropertyではなく、メンバ変数ポインタで管理するPropertyDiscripterを保持する設計に変更
//				この変更によってコピー・ムーブをした際にもアドレスが破綻しないようになりました。
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>
#include <string>
#include <vector>

#include "Property.h"
#include "Common/CheckVector.h"
#include "Assets/Objects/Handle.h"
#include "Common/ObjectReference.h"
#include "EnumRegistry.h"
#include "ArrayRegistry.h"
#include "AssetPropertyRegistry.h"

// マクロ
#define ADD_PROPERTY(Class, Property) (AddProperty(#Property, &Class::Property))

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
		std::vector<PropertyDiscripter> m_properties;

	protected:
		// コンストラクタ
		PropertyObject()
			: m_properties{}
		{}
		// コピー
		PropertyObject(const PropertyObject&) = default;
		PropertyObject& operator=(const PropertyObject&) = default;

		// デストラクタ
		virtual ~PropertyObject() = default;

	public:
		// プロパティを取得する関数
		std::vector<Property> GetProperties() 
		{
			std::vector<Property> properties{};

			// DiscripterからPropertyを構築
			for (auto& d : m_properties)
			{
				properties.push_back(d.Resolve(this));
			}

			return properties;
		}

	protected:
		// プロパティの追加関数
		template<typename Class, typename T>
		void AddProperty(const std::string& name, T Class::* memberPtr)
		{
			PropertyDiscripter prop;
			prop.name = name;
			prop.type = GetPropertyType<T>();
			prop.typeIndex = RegisterType<T>();

			// ラムダ式の登録
			prop.getAddress = [memberPtr](PropertyObject* obj) -> void*
				{
					Class* derived = static_cast<Class*>(obj);
					return static_cast<void*>(&(derived->*memberPtr));
				};

			// 配列に追加
			m_properties.push_back(prop);
		}

	private:
		// 型登録関数
		template<typename T>
		std::type_index RegisterType()
		{
			// 列挙型なら
			if constexpr (std::is_enum_v<T>)
			{
				// 登録
				EnumRegistry::Instance().Register<T>();

				// タイプインデックスを保存
				return std::type_index(typeid(T));
			}
			// AssetHandleなら
			else if constexpr (IsHandle_v<T>)
			{
				// 登録
				AssetPropertyRegistry::Instance().Register<typename T::value_type>();

				// タイプインデックスを返す
				return std::type_index(typeid(typename T::value_type));
			}
			// vectorなら
			else if constexpr (is_vector<T>::value)
			{
				// 配列のテンプレート型
				using ElementType = typename T::value_type;

				// 登録
				ArrayRegistry::Instance().Register<T>(GetPropertyType<ElementType>());

				// 管理対象の型も登録
				RegisterType<ElementType>();

				// タイプインデックスを返す
				return std::type_index(typeid(T));
			}
			else return std::type_index(typeid(void));
		}

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
			// vector
			else if constexpr (is_vector<T>::value) return PropertyType::Array;

			// その他
			else return PropertyType::None;
		}
	};
} // namespace REngine
