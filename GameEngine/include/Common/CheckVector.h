/**
 * @file    CheckVector.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/03
 *
 * @brief   型がvectorかどうかを調べる
 */

#pragma once

#include <vector>
#include <type_traits>

namespace RHEngine
{
	template <typename T>
	struct is_vector : std::false_type {
		using value_type = void;
	};

	template <typename U, typename Alloc>
	struct is_vector<std::vector<U, Alloc>> : std::true_type {
		using value_type = U;
	};
}
