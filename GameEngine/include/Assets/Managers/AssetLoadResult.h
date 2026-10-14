//====================================================//
// ファイル名   : AssetLoadResult.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/12
//
// 概要 : アセットのロード結果を格納する構造体
//
// 更新履歴 :
// 2026/10/12 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>

#include "Assets/Objects/Handle.h"
#include "Assets/Objects/AssetBase.h"

namespace REngine
{
	//====================================================//
	// 構造体宣言
	//====================================================//

	// サブアセットの構造体
	struct SubAssetEntry
	{
		// サブアセット名
		std::string name;

		// アセット本体
		std::unique_ptr<AssetBase> asset;

		// アセットハンドル
		UnTypeHandle handle;
	};

	// ロード結果の構造体
	struct LoadResult
	{
		// アセット本体
		std::unique_ptr<AssetBase> asset;

		// アセットハンドル
		UnTypeHandle handle;

		// サブアセット
		std::vector<SubAssetEntry> subAssets;

		// 成功フラグ
		bool success;
	};
}
