/**
 * @file    MaterialSaver.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/10
 *
 * @brief   マテリアルを保存する
 */

#pragma once

#include <filesystem>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	class AssetContext;

	namespace Saver
	{
		/**
		 * @brief マテリアルの保存をする
		 */
		class MaterialSaver
		{
		public:

			/**
			 * @brief マテリアルを保存する
			 * @param asset 対象のマテリアル
			 * @param path 保存先のパス
			 * @param ctx アセットコンテキスト
			 */
			static void Save(AssetBase* asset, const std::filesystem::path& path, AssetContext& ctx);
		};
	}
}	// namespace RHEngine
