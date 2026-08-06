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

#include "Assets/Types/Texture.h"
#include "Assets/Types/Model.h"
#include "Assets/Types/Font.h"
#include "Assets/Types/Prefab.h"
#include "Assets/Types/AudioClip.h"

#include "Editor/Loader/ObjectLoader.h"

namespace REngine
{
	namespace Loader
	{		
		// 各ロード関数

		// Texture
		std::unique_ptr<Texture> TextureLoader(const std::filesystem::path& path);

		// Model
		std::unique_ptr<Model> ModelLoader(const std::filesystem::path& path);

		// Font
		std::unique_ptr<Font> FontLoader(const std::filesystem::path& path);

		// Prefab
		std::unique_ptr<Prefab> PrefabLoader(const std::filesystem::path& path);

		// AudioClip
		std::unique_ptr<AudioClip> AudioClipLoader(const std::filesystem::path& path);

		// Propertyとして扱う場合
		template<typename T>
		requires std::derived_from<T, AssetBase>
		std::unique_ptr<T> AssetLoaderAsProperty(const std::filesystem::path& path, AssetManager& assetManager)
		{
			// ローダーを生成
			ObjectLoader loader(assetManager);

			// アセットを生成
			std::unique_ptr<T> asset = std::make_unique<T>();

			// ファイルからロード
			loader.LoadPropertyFromFile(path, asset.get());

			// 返す
			return std::move(asset);
		}
	}
}	// namespace REngine
