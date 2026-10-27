/**
 * @file    Property.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/21
 *
 * @brief   プロパティに関する情報を宣言したヘッダ
 */

#pragma once

#include <functional>
#include <typeindex>
#include <string>

namespace RHEngine
{
	/**
	 * @brief プロパティとして扱うことが出来るタイプの列挙型
	 */
	enum class PropertyType
	{
		/// @name 変数
		/// @{

		None,
		Int,		///< int
		Float,		///< float
		Bool,		///< bool
		String,		///< std::string
		Vector2,	///< DirectX::SimpleMath::Vector2
		Vector3,	///< DirectX::SimpleMath::Vector3
		Vector4,	///< DirectX::SimpleMath::Vector4
		Quaternion,	///< DirectX::SimpleMath::Quaternion
		Color,		///< DirectX::SimpleMath::Color
		Object,		///< PropertyObject派生クラス
		Enum,		///< 列挙型
		AssetHandle,///< アセットハンドル
		ObjectRef,	///< オブジェクト参照
		Array,		///< 配列
		Range,		///< 範囲

		/// @}

		/// @name 装飾
		/// @{

		Header,		///< ヘッダー

		/// @}
	};

	/**
	 * @brief プロパティ
	 */
	struct Property
	{
		std::string name = "";	///< プロパティ名

		PropertyType type = PropertyType::None;	///< プロパティタイプ

		void* value = nullptr;	///< 本体のアドレス

		std::type_index typeIndex = std::type_index(typeid(void));	///< プロパティ型のtype_index
	};

	class PropertyObject;

	/**
	 * @brief メンバ変数ポインタから実アドレスを取得するプロパティのディスクリプタ
	 */
	struct PropertyDiscripter
	{
		std::string name = "";	///< プロパティ名

		PropertyType type = PropertyType::None;	///< プロパティタイプ
		
		std::type_index typeIndex = std::type_index(typeid(void));	///< プロパティ型のタイプインデックス

		std::function<void* (PropertyObject*)> getAddress;	///< インスタンスを渡して実アドレスを取得する関数オブジェクト

		/**
		 * @brief プロパティ型へ解決する
		 * @param obj 所有者のポインタ
		 * @return 解決済みプロパティ
		 */
		Property Resolve(PropertyObject* obj) const
		{
			return Property{ name, type, getAddress ? getAddress(obj) : nullptr, typeIndex };
		}
	};
}	// namespace RHEngine
