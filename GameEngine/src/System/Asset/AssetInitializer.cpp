//====================================================//
// ファイル名  : AssetInitializer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "AssetInitializer.h"

#include "System/GraphicsManager.h"
#include "Assets/Types/PhysicsMaterial.h"

#include "AssetLoaders.h"
#include "AssetSaver.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void REngine::AssetInitializer::AssetInitialize(AssetManager& assetManager)
	{
		// デバイスを取得
		auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// Textureの登録
		assetManager.Registry<Texture>(
			"Texture",
			Loader::TextureLoader,
			nullptr, 
			false,
			{ L".png", L".jpeg", L".bmp", L".tiff", L".gif", L".dds" });

		// Modelの登録
		assetManager.Registry<Model>(
			"Model",
			Loader::ModelLoader,
			nullptr, 
			false,
			{ L".cmo", L".sdkmesh" });

		// Fontの登録
		assetManager.Registry<Font>(
			"Font",
			Loader::FontLoader,
			nullptr, 
			false,
			{ L".spritefont" });

		// Prefabの登録
		assetManager.Registry<Prefab>(
			"Prefab",
			Loader::PrefabLoader,
			Saver::PrefabSaver, 
			false,
			{ L".gameobject" });

		// AudioClipの登録
		assetManager.Registry<AudioClip>(
			"AudioClip",
			Loader::AudioClipLoader,
			nullptr, 
			false,
			{ L".wav" });

		// PhysicsMaterialの登録
		assetManager.Registry<PhysicsMaterial>(
			"PhysicsMaterial",
			[&assetManager](const std::wstring& path) { return Loader::AssetLoaderAsProperty<PhysicsMaterial>(path, assetManager); },
			[&assetManager](AssetBase* base, const std::wstring& path) { return Saver::AssetSaverAsProperty(base, path, assetManager); },
			true,
			{ L".physicsmaterial" });
	}
}
