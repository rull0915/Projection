/**
 * @file    ShaderLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/04
 *
 * @brief   シェーダーアセットのロードに関連する関数群
 */

#pragma once

#include <filesystem>

#include "Assets/Managers/AssetContext.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	class ShaderAsset;

	namespace Loader
	{
		/**
		 * @brief シェーダーアセットを読み込む
		 * @param path 読み込む対象のコンパイル済みシェーダーファイルパス
		 * @details ファイルパスの拡張子は、ファイル名.シェーダーステージ.csoとする
		 *			例: AAA.ps.cso, AAA.vs.cso
		 * @param ctx アセット用コンテキスト
		 * @return 読み込み結果
		 */
		LoadResult ShaderLoader(const std::filesystem::path& path, AssetContext& ctx);
	}
}	// namespace RHEngine
