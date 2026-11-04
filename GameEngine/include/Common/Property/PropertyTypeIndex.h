/**
 * @file    PropertyTypeIndex.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/09
 *
 * @brief   プロパティで使用するタイプインデックスを取得するクラス
 */

#pragma once

#include <typeindex>

#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	/**
	 * @brief プロパティに対応するtype_indexを管理する
	 */
	class PropertyTypeIndex
	{
	public:
		/**
		 * @brief 型に対応するプロパティ用のtype_indexを取得する
		 * @tparam T 調べる型
		 * @return 対応するtype_index
		 */
		template<typename T>
		static std::type_index GetTypeIndex()
		{
			// アセットハンドルの場合
			if constexpr (IsHandle_v<T>)
			{
				// アセットの型を返す
				return std::type_index(typeid(T::value_type));
			}
			// それ以外はそのままのtype_indexを扱う
			else
			{
				return std::type_index(typeid(T));
			}
		}
	};
}
