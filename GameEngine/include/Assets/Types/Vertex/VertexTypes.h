//====================================================//
// ファイル名   : VertexTypes.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/24
//
// 概要 : 頂点構造体の制限を宣言したヘッダ
//
// 更新履歴 :
// 2026/09/24 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <concepts>
#include <vector>

#include "VertexElementInfo.h"

namespace REngine
{
	// staticなGetLayout関数を持っているかをチェックするConcept
	template <typename T>
	concept VertexType = requires {
		{ T::GetLayout() } -> std::convertible_to<const std::vector<VertexElementInfo>&>;
	};
}
