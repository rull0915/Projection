/**
 * @file    HandlePayload.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/27
 *
 * @brief   ハンドルを受け渡しする用の構造体
 */

#pragma once

#ifdef ENGINE_GUI

#include <typeindex>

#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	/**
	 * @brief ハンドルをウィンドウ間で受け渡しする用の構造体
	 */
	struct HandlePayload
	{
		std::type_index type;	///< 型
		UnTypeHandle handle;	///< ハンドル
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
