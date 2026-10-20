//====================================================//
// ファイル名   : ShaderLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要 : シェーダーアセットのロードに関連する関数群
//
// 更新履歴 :
// 2026/08/04 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>
#include <filesystem>

#include "Assets/Managers/AssetContext.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	class ShaderAsset;

	// ロード関数群名前空間
	namespace Loader
	{
		// ロード関数
		LoadResult ShaderLoader(const std::filesystem::path& path, AssetContext& ctx);
	}
}
