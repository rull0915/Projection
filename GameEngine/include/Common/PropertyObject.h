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
	std::vector<Property> m_propaties;

protected:
	// コンストラクタ
	PropertyObject()
		: m_propaties{}
	{}

public:
	// プロパティを取得する関数
	const std::vector<Property>& GetPropaties() const { return m_propaties; }

protected:
	// プロパティの追加関数
	template<typename T>
	void AddProperty(std::string name, T* value)
	{
		// 追加
		m_propaties.push_back({name, GetPropertyType<T>(), value});
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

		// その他
		else return PropertyType::None;
	}
};
