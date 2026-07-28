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

#include "Common/GetExtension.h"
#include "System/GraphicsManager.h"
#include "System/ResourceManager.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

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
		assetManager.Registry<Texture>("Texture", Loader::TextureLoader, { L".png", L".jpeg", L".bmp", L".tiff", L".gif", L".dds" });

		// Modelの登録
		assetManager.Registry<Model>("Model", Loader::ModelLoader, { L".cmo", L".sdkmesh" });

		// Fontの登録
		assetManager.Registry<Font>("Font", Loader::FontLoader, { L".spritefont" });
		
		// Prefabの登録
		assetManager.Registry<Prefab>("Prefab", Loader::PrefabLoader, { L".gameobject" });

		// AudioClipの登録
		assetManager.Registry<AudioClip>("AudioClip", Loader::AudioClipLoader, { L".wav" });
	}

	std::unique_ptr<Texture> REngine::Loader::TextureLoader(const std::wstring& path)
	{
		// デバイスを取得
		auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// テクスチャを用意
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		HRESULT hr;

		// キーの拡張子を取得
		std::string extension = GetExtension::Get(path);

		// ddsなら
		if (extension == ".dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(
				device,                 // ID3D11Device*
				path.data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}
		// それ以外なら
		else
		{
			hr = DirectX::CreateWICTextureFromFile(
				device,                 // ID3D11Device*
				path.data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}

		// 失敗したら
		if (FAILED(hr))
		{
			// nullを返す
			return nullptr;
		}

		// Textureの生成
		return std::make_unique<Texture>(std::move(texture));
	}

	std::unique_ptr<Model> Loader::ModelLoader(const std::wstring& path)
	{
		// デバイスを取得
		auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// EffectFactoryを取得
		auto fx = ResourceManager::Instance().GetEffectFactory();

		// キーの拡張子を取得
		std::string extension = GetExtension::Get(path);

		std::unique_ptr<DirectX::Model> model;

		// CMOファイルの場合
		if (extension == ".cmo")
		{
			model = DirectX::Model::CreateFromCMO(device, path.c_str(), *fx);
		}
		// SDKMESHファイルの場合
		else if (extension == ".sdkmesh")
		{
			model = DirectX::Model::CreateFromSDKMESH(device, path.c_str(), *fx);
		}

		// Modelの生成
		return std::make_unique<Model>(std::move(model));
	}

	std::unique_ptr<Font> Loader::FontLoader(const std::wstring& path)
	{
		// デバイスを取得
		auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// 生成
		std::unique_ptr<DirectX::SpriteFont> font = std::make_unique<DirectX::SpriteFont>(device, path.c_str());

		// デフォルト文字を「?」に設定
		font->SetDefaultCharacter(L'?');

		float height;

		RECT result = font->MeasureDrawBounds(L"A", DirectX::XMFLOAT2(0.f, 0.f));
		height = std::fabsf(static_cast<float>(result.bottom - result.top));

		// 返す
		return std::make_unique<Font>(std::move(font), height);
	}

	std::unique_ptr<Prefab> Loader::PrefabLoader(const std::wstring& path)
	{
		// 返す
		return std::make_unique<Prefab>(std::filesystem::path(path));
	}

	std::unique_ptr<AudioClip> Loader::AudioClipLoader(const std::wstring& path)
	{
		// AudioEngineを取得
		auto ae = ResourceManager::Instance().GetAudioEngine();

		// 生成
		auto sound = std::make_unique<DirectX::SoundEffect>(ae, path.c_str());

		// 返す
		return std::make_unique<AudioClip>(std::move(sound));
	}
}
