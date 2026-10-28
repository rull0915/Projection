/**
 * @file    AssetInitializer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 */

#include "pch.h"
#include "AssetInitializer.h"

#include "System/Render/GraphicsManager.h"
#include "Assets/Types/PhysicsMaterial.h"
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Types/Material/MaterialAsset.h"

#include "Assets/Types/Font.h"
#include "Assets/Types/AudioClip.h"
#include "Assets/Types/Model/Model.h"
#include "Assets/Types/Model/Mesh.h"

#include "Assets/Types/Shader/ShaderLoader.h"
#include "Assets/System/Material/MaterialLoader.h"
#include "Assets/System/Material/MaterialSaver.h"
#include "Assets/System/Model/OBJLoader.h"

#include "AssetLoaders.h"
#include "AssetSaver.h"

namespace RHEngine
{
	void RHEngine::AssetInitializer::AssetInitialize(AssetManager& assetManager)
	{
		// デバイスを取得
		auto pDevice = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

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
			[&assetManager](const std::filesystem::path& path, AssetContext& ctx) { return Loader::AssetLoaderAsProperty<PhysicsMaterial>(path, assetManager); },
			[&assetManager](AssetBase* base, const std::filesystem::path& path, AssetContext& ctx) { return Saver::AssetSaverAsProperty(base, path, assetManager); },
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
			Loader::MaterialLoader::Load,
			Saver::MaterialSaver::Save,
			true,
			false,
			{ L".mat" });
	}
}
