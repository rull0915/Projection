//====================================================//
// ファイル名   : Property.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : プロパティに関する情報を宣言したヘッダ
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <typeindex>
#include <string>

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	// 型の列挙型
	enum class PropertyType
	{
		None,
		Int,		// int
		Float,		// float
		Bool,		// bool
		String,		// std::string
		Vector2,	// DirectX::SimpleMath::Vector2
		Vector3,	// DirectX::SimpleMath::Vector3
		Quaternion,	// DirectX::SimpleMath::Quaternion
		Color,		// DirectX::SimpleMath::Color
		Object,		// PropertyObject派生クラス
		Enum,		// 列挙型
		AssetHandle,// アセットハンドル
		ObjectRef,	// オブジェクト参照
		Array,		// 配列
	};

	// プロパティ
	struct Property
	{
		// プロパティ名
		std::string name;
		// プロパティタイプ
		PropertyType type;
		// ポインタ
		void* value;
		// タイプインデックス
		std::type_index typeIndex = std::type_index(typeid(void));
	};

	// 前方宣言
	class PropertyObject;

	// メンバ変数ポインタから実アドレスを取得できるコピー可能なプロパティ
	struct PropertyDiscripter
	{
		// プロパティ名
		std::string name = "";
		// プロパティタイプ
		PropertyType type = PropertyType::None;
		// タイプインデックス
		std::type_index typeIndex = std::type_index(typeid(void));

		// インスタンスを渡して実アドレスを取得する関数オブジェクト
		std::function<void* (PropertyObject*)> getAddress;

		// 実データに解決する関数
		Property Resolve(PropertyObject* obj) const
		{
			return Property{ name, type, getAddress(obj), typeIndex };
		}
	};
}	// namespace REngine
