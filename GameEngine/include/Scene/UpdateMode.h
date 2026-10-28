/**
 * @file    UpdateMode.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/12
 *
 * @brief   更新モードの列挙型
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 更新モード
	 */
	enum class UpdateMode
	{
		Play,	// 通常時
		Pause,	// ポーズ
		Edit,	// 編集中
	};
}	// namespace RHEngine
