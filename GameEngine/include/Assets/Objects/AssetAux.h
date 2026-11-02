/**
 * @file    AssetAux.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   アセットの補助構造体
 */

#pragma once

#include <string>
#include <vector>

#include "Common/UUID.h"

namespace RHEngine
{
	/**
	 * @brief サブアセットの情報
	 */
	struct SubAssetInfo
	{		
		std::string name = "";	///< アセット名

		UUID uuid = UUID_NONE;	///< 対応するUUID
		
		std::string assetType = "";	///< アセットタイプ名
	};

	/**
	 * @brief アセットの補助構造体
	 */
	struct AssetAux
	{
		UUID uuid = UUID_NONE;	///< 対応するUUID

		std::string assetType = "";	///< アセットタイプ名

		std::vector<SubAssetInfo> subAssets;	///< サブアセット情報のリスト
	};
}	// namespace RHEngine
