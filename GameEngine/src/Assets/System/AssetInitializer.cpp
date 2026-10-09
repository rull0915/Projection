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
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Types/MaterialAsset.h"
#include "Assets/Types/Model/Model.h"
#include "Assets/Types/Model/Mesh.h"

#include "Assets/Types/Shader/ShaderLoader.h"
#include "Assets/System/Model/OBJLoader.h"

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
			false,
			{ L".png", L".jpeg", L".bmp", L".tiff", L".gif", L".dds" });

		// Modelの登録
		assetManager.Registry<Model>(
			"Model",
			Loader::OBJLoader::Load,
			nullptr, 
			false,
			true,
			{ L".obj" });

		// Meshの登録
		assetManager.Registry<Mesh>(
			"Mesh",
			nullptr,
			nullptr, 
			false,
			true,
			{ L".mesh" });

		// Fontの登録
		assetManager.Registry<Font>(
			"Font",
			Loader::FontLoader,
			nullptr, 
			false,
			false,
			{ L".spritefont" });

		// Prefabの登録
		assetManager.Registry<Prefab>(
			"Prefab",
			Loader::PrefabLoader,
			Saver::PrefabSaver, 
			false,
			false,
			{ L".gameobject" });

		// AudioClipの登録
		assetManager.Registry<AudioClip>(
			"AudioClip",
			Loader::AudioClipLoader,
			nullptr, 
			false,
			false,
			{ L".wav" });

		// PhysicsMaterialの登録
		assetManager.Registry<PhysicsMaterial>(
			"PhysicsMaterial",
			[&assetManager](const std::filesystem::path& path, AssetLoadContext& ctx) { return Loader::AssetLoaderAsProperty<PhysicsMaterial>(path, assetManager); },
			[&assetManager](AssetBase* base, const std::filesystem::path& path) { return Saver::AssetSaverAsProperty(base, path, assetManager); },
			true,
			false,
			{ L".physicsmaterial" });

		// Shaderの登録
		assetManager.Registry<ShaderAsset>(
			"Shader",
			Loader::ShaderLoader,
			nullptr,
			false,
			false,
			{ L".cso" });

		// Materialの登録
		assetManager.Registry<MaterialAsset>(
			"Material",
			[&assetManager](const std::filesystem::path& path, AssetLoadContext& ctx) { return Loader::AssetLoaderAsProperty<MaterialAsset>(path, assetManager); },
			[&assetManager](AssetBase* base, const std::filesystem::path& path) { return Saver::AssetSaverAsProperty(base, path, assetManager); },
			true,
			false,
			{ L".mat" });
	}
}
