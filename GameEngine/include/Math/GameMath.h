/**
 * @file    GameMath.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/28
 *
 * @brief   数学関数をまとめたヘッダ
 */

#pragma once

#include <type_traits>

namespace RHEngine
{
	namespace Math
	{
		/**
		 * @brief 符号を取得する
		 * @tparam T 取得する数値型
		 * @param value 取得対象の数値
		 * @return 符号 (1 or -1 or 0)
		 */
		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		int Sign(T value)
		{
			if (value > 0) return 1;
			if (value < 0) return -1;
			return 0;
		}
	}
}	// namespace RHEngine
