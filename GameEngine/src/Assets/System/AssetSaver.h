/**
 * @file    AssetSaver.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/02
 *
 * @brief   アセットの保存関数をまとめたヘッダ
 */

#pragma once

#include <filesystem>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	class AssetContext;
	class AssetManager;

	namespace Saver
	{		
		/// @name 保存関数
		/// @{

		/**
		 * @brief プレハブ保存関数
		 * @param pAsset 保存するプレハブ
		 * @param path 保存先のパス
		 */
		void PrefabSaver(AssetBase* pAsset, const std::filesystem::path& path, AssetContext& ctx);

		/**
		 * @brief アセットをPropertyObjectとして保存する
		 * @param pAsset 保存するアセット
		 * @param path 保存先のパス
		 * @param assetManager アセットマネージャー
		 */
		void AssetSaverAsProperty(AssetBase* pAsset, const std::filesystem::path& path, const AssetManager& assetManager);

		/// @}
	}
}	// namespace RHEngine
