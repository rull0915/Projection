/**
 * @file    ComponentTypeId.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/26
 *
 * @brief   コンポーネントのTypeIdを宣言したヘッダ
 */

#pragma once

namespace RHEngine
{
	namespace Component
	{
		/**
		 * @brief アドレスをIDとして扱う
		 * @details 重複が存在せず、等価などのチェックが高速であるため void* を使用
		 */
		using TypeId = const void*;
	}
}
