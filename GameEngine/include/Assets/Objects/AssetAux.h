//====================================================//
// ファイル名   : AssetAux.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : アセットの補助構造体
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include "Common/UUID.h"

namespace REngine
{
	//====================================================//
	// 構造体宣言
	//====================================================//

	struct SubAssetInfo
	{
		// 識別用名前
		std::string name = "";

		// UUID
		UUID uuid = UUID_NONE;
		
		// アセットの種類
		std::string assetType = "";
	};

	struct AssetAux
	{
		// UUID
		UUID uuid = UUID_NONE;

		// アセットの種類
		std::string assetType = "";

		// サブアセットの配列
		std::vector<SubAssetInfo> subAssets;
	};
}	// namespace REngine
