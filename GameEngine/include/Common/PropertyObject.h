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

// マクロ
#define ADD_PROPERTY(property) (AddProperty(#property, &property))

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	// 型の列挙型
	enum class PropertyType
	{
		None,
		Int,
		Float,
		Bool,
		String,
		Vector2,
		Vector3,
		Quaternion,
		Color,
		Object,
	};

	// プロパティ
	struct Property
	{
		std::string name;
		PropertyType type;
		void* value;
	};

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
			// 追加
			m_properties.push_back({ name, GetPropertyType<T>(), value });
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

			// その他
			else return PropertyType::None;
		}
	};
} // namespace REngine
