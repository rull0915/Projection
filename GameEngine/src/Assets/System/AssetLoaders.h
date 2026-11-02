/**
 * @file    AssetLoaders.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/02
 *
 * @brief   アセットのロード関数をまとめたヘッダ
 */

#pragma once

#include "Editor/Loader/ObjectLoader.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	namespace Loader
	{		
		/// @name ロード関数
		/// @{

		/**
		 * @brief テクスチャの読み込み
		 * @param path 読み込むファイルパス
		 * @return 読み込み結果
		 */
		LoadResult TextureLoader(const std::filesystem::path& path, AssetContext& ctx);

		/**
		 * @brief フォントの読み込み
		 * @param path 読み込むファイルパス
		 * @return 読み込み結果
		 */
		LoadResult FontLoader(const std::filesystem::path& path, AssetContext& ctx);

		/**
		 * @brief プレハブの読み込み
		 * @param path 読み込むファイルパス
		 * @return 読み込み結果
		 */
		LoadResult PrefabLoader(const std::filesystem::path& path, AssetContext& ctx);

		/**
		 * @brief サウンドファイルの読み込み
		 * @param path 読み込むファイルパス
		 * @return 読み込み結果
		 */
		LoadResult AudioClipLoader(const std::filesystem::path& path, AssetContext& ctx);

		/**
		 * @brief アセットをPropertyObjectとして読み込む
		 * @tparam T アセットの型
		 * @param path 読み込むファイルパス
		 * @param assetManager アセットマネージャー
		 * @return 読み込み結果
		 */
		template<typename T>
		requires std::derived_from<T, AssetBase>
		LoadResult AssetLoaderAsProperty(const std::filesystem::path& path, AssetManager& assetManager)
		{
			// リザルト
			LoadResult result{};

			// ローダーを生成
			ObjectLoader loader(assetManager);

			// アセットを生成
			result.asset = std::make_unique<T>();

			// ファイルからロード
			loader.LoadPropertyFromFile(path, result.asset.get());

			result.success = true;

			// 返す
			return result;
		}

		/// @}
	}
}	// namespace RHEngine
