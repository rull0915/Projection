//====================================================//
// ファイル名   : PropatyObject.h
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
#define ADD_PROPATY(propaty) (AddPropaty(#propaty, &propaty))

//====================================================//
// クラス宣言
//====================================================//


// 型の列挙型
enum class PropatyType
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
struct Propaty
{
	std::string name;
	PropatyType type;
	void* value;
};

// プロパティを扱うオブジェクトの基底クラス
class PropatyObject
{
private:
	// プロパティ一覧
	std::vector<Propaty> m_propaties;

protected:
	// コンストラクタ
	PropatyObject();

public:
	// プロパティを取得する関数
	const std::vector<Propaty>& GetPropaties() const { return m_propaties; }

protected:
	// プロパティの追加関数
	template<typename T>
	void AddPropaty(std::string name, T* value)
	{
		// 追加
		m_propaties.push_back({name, GetPropatyType<T>(), value});
	}

private:
	// 型からタイプを取得する関数
	template<typename T>
	PropatyType GetPropatyType()
	{
		// bool
		if constexpr (std::is_same_v<T, bool>) return PropatyType::Bool;
		// int
		else if constexpr (std::is_same_v<T, int>) return PropatyType::Int;
		// float
		else if constexpr (std::is_same_v<T, float>) return PropatyType::Float;
		// string
		else if constexpr (std::is_same_v<T, std::string>) return PropatyType::String;
		// Vector2
		else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector2>) return PropatyType::Vector2;
		// Vector3
		else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector3>) return PropatyType::Vector3;
		// Quaternion
		else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Quaternion>) return PropatyType::Quaternion;

		// その他
		else return PropatyType::None;
	}
};
