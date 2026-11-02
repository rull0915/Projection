/**
 * @file    LoadStatus.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   読み込み状態を宣言したヘッダ
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 読み込み状態
	 */
	enum class LoadStatus
	{
		Unloaded, // 開始前
		Loading,  // 読み込み中
		Loaded,   // 成功
		Failed    // 失敗
	};
}	// namespace RHEngine
