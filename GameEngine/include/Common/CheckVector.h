//====================================================//
// ファイル名   : CheckVector.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/03
//
// 概要 : vectorかどうかを調べるためのヘッダー
//
// 更新履歴 :
// 2026/09/03 新規作成
//====================================================//

#pragma once

#include <vector>

namespace REngine
{
	// vector以外のテンプレート
	template <typename T>
	struct is_vector : std::false_type {
		using value_type = void;
	};

	// std::vector
	template <typename U, typename Alloc>
	struct is_vector<std::vector<U, Alloc>> : std::true_type {
		using value_type = U;
	};
}
