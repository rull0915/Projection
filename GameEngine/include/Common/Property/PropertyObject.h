/**
 * @file    PropertyObject.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/02
 *
 * @brief   プロパティを所有するクラス
 */

#pragma once

#include <type_traits>
#include <string>
#include <vector>

#include "Property.h"
#include "PropertyTypeIndex.h"

#include "Common/CheckVector.h"
#include "Assets/Objects/Handle.h"
#include "Common/ObjectReference.h"
#include "Math/Range.h"

#include "EnumRegistry.h"
#include "ArrayRegistry.h"
#include "AssetPropertyRegistry.h"

#define ADD_PROPERTY(Class, Property) (AddProperty(#Property, &Class::Property))
#define ADD_HEADER(Name) (AddDecoration(Name, RHEngine::PropertyType::Header))

namespace RHEngine
{
	/**
	 * @brief プロパティを扱うオブジェクトの基底となるクラス
	 * @details プロパティディスクリプタを保持し、プロパティの一覧を取得できるようにする
	 */
	class PropertyObject
	{
	private:
		/** @brief プロパティディスクリプタのリスト */
		std::vector<PropertyDiscripter> m_properties;

	protected:
		/// @name ライフサイクル
		/// @{

		PropertyObject() = default;

		virtual ~PropertyObject() = default;

		/// @}

	public:
		/**
		 * @brief プロパティリストを取得する
		 * @return プロパティリスト
		 */
		virtual std::vector<Property> GetProperties() 
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

		/**
		 * @brief プロパティを追加する
		 * @tparam Class プロパティ本体を所有するクラス
		 * @tparam T プロパティ本体の型
		 * @param name プロパティ名
		 * @param memberPtr プロパティへのメンバ変数ポインタ
		 */
		template<typename Class, typename T>
		void AddProperty(const std::string& name, T Class::* memberPtr)
		{
			PropertyDiscripter prop;
			prop.name = name;
			prop.type = GetPropertyType<T>();
			prop.typeIndex = PropertyTypeIndex::GetTypeIndex<T>();

			// 型の登録
			RegisterType<T>();

			// ラムダ式の登録
			prop.getAddress = [memberPtr](PropertyObject* obj) -> void*
				{
					Class* derived = static_cast<Class*>(obj);
					return static_cast<void*>(&(derived->*memberPtr));
				};

			// 配列に追加
			m_properties.push_back(prop);
		}

		/**
		 * @brief 装飾用プロパティを追加する
		 * @param name プロパティ名
		 * @param type プロパティタイプ
		 */
		void AddDecoration(const std::string& name, PropertyType type)
		{
			PropertyDiscripter prop;
			prop.name = name;
			prop.type = type;
			prop.typeIndex = std::type_index(typeid(void));
			prop.getAddress = nullptr;

			// 配列に追加
			m_properties.push_back(prop);
		}

		/**
		 * @brief 型を各レジストリへと登録する
		 * @tparam T 登録対象の型
		 */
		template<typename T>
		void RegisterType()
		{
			// 列挙型なら
			if constexpr (std::is_enum_v<T>)
			{
				// 登録
				EnumRegistry::Instance().Register<T>();
			}
			// AssetHandleなら
			else if constexpr (IsHandle_v<T>)
			{
				// 登録
				AssetPropertyRegistry::Instance().Register<typename T::value_type> ();
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
			}
		}

		/**
		 * @brief 型からプロパティタイプを取得する
		 * @tparam T 調べる型
		 * @return 対応するプロパティタイプ
		 */
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
			// Vector4
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector4>) return PropertyType::Vector4;
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
			// Range
			else if constexpr (std::is_same_v<T, Range>) return PropertyType::Range;

			// その他
			else return PropertyType::None;
		}
	};
} // namespace RHEngine
