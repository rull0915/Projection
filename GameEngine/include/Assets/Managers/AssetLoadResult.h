/**
 * @file    AssetLoadResult.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/12
 *
 * @brief   アセットのロード結果を格納する構造体
 */

#pragma once

#include <memory>

#include "Assets/Objects/Handle.h"
#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief サブアセットの情報
	 */
	struct SubAssetEntry
	{
		std::string name;	///< アセット名

		std::unique_ptr<AssetBase> asset;	///< アセット本体
			
		UnTypeHandle handle;	///< アセットハンドル
	};

	/**
	 * @brief ロード結果
	 */
	struct LoadResult
	{
		std::unique_ptr<AssetBase> asset;	///< アセット本体

		UnTypeHandle handle;	///< アセットハンドル

		std::vector<SubAssetEntry> subAssets;	///< 自身が持つサブアセット群

		bool success;	///< 読み込みが成功したかどうか
	};
}	// namespace RHEngine
