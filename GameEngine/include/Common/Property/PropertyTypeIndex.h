//====================================================//
// ファイル名   : PropertyTypeIndex.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/09
//
// 概要 : プロパティで使用するタイプインデックスを取得するクラス
//
// 更新履歴 :
// 2026/10/09 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <typeindex>
#include "Assets/Objects/Handle.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class PropertyTypeIndex
	{
	public:
		/// <summary>
		/// 型ごとに対応するtype_indexを取得する関数
		/// </summary>
		/// <typeparam name="T">型</typeparam>
		/// <returns></returns>
		template<typename T>
		static std::type_index GetTypeIndex()
		{
			// アセットハンドルの場合
			if constexpr (IsHandle_v<T>)
			{
				// アセットの型を返す
				return std::type_index(typeid(T::value_type));
			}
			else
			{
				return std::type_index(typeid(T));
			}
		}
	};
}
