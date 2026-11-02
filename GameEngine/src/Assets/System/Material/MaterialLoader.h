/**
 * @file    MaterialLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/10
 *
 * @brief   マテリアルの読み込みを行う
 */

#pragma once

#include <filesystem>

#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	class AssetContext;

	namespace Loader
	{
		/**
		 * @brief マテリアルをファイルから読み込む
		 */
		class MaterialLoader
		{
		public:

			/**
			 * @brief マテリアルを読み込む
			 * @param path マテリアルのファイルパス
			 * @param ctx アセットコンテキスト
			 * @return 読み込んだ結果
			 */
			static LoadResult Load(const std::filesystem::path& path, AssetContext& ctx);
		};
	}
}	// namespace RHEngine
