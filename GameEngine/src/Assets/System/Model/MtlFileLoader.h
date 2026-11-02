/**
 * @file    MtlFileLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/13
 *
 * @brief   .mtlファイルからマテリアルを生成するクラス
 */

#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/AssetContext.h"

namespace RHEngine
{
	/**
	 * @brief .mtlファイルを読み込んで対応するMaterialを生成する
	 */
	class MtlFileLoader
	{
	public:

		/**
		 * @brief .mtlファイルを読み込む
		 * @details 1つの.mtlファイルに複数のマテリアルが存在することもあるため、マテリアルのリストを戻り値とする
		 * @param path 読み込むファイルパス
		 * @param ctx アセットコンテキスト
		 * @return 生成したマテリアルのリスト (マテリアルとのペア)
		 */
		static std::unordered_map<std::string, MaterialAsset> LoadMTL(const std::filesystem::path& path, AssetContext& ctx);
	};
}	// namespace RHEngine
