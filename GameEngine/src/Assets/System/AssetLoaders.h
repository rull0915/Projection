//====================================================//
// ファイル名   : AssetLoaders.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/02
//
// 概要 : アセットのロード関数をまとめたヘッダ
//
// 更新履歴 :
// 2026/08/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "Editor/Loader/ObjectLoader.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace REngine
{
	namespace Loader
	{		
		// 各ロード関数

		// Texture
		LoadResult TextureLoader(const std::filesystem::path& path, AssetContext& ctx);

		// Font
		LoadResult FontLoader(const std::filesystem::path& path, AssetContext& ctx);

		// Prefab
		LoadResult PrefabLoader(const std::filesystem::path& path, AssetContext& ctx);

		// AudioClip
		LoadResult AudioClipLoader(const std::filesystem::path& path, AssetContext& ctx);

		// Propertyとして扱う場合
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

			result.scuccess = true;

			// 返す
			return result;
		}
	}
}	// namespace REngine
