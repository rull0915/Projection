//====================================================//
// ファイル名  : AssetLoaders.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/02
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "AssetLoaders.h"

#include "Assets/Types/Texture.h"
#include "Assets/Types/Font.h"
#include "Assets/Types/Prefab.h"
#include "Assets/Types/AudioClip.h"

#include "System/GraphicsManager.h"
#include "System/ResourceManager.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	LoadResult REngine::Loader::TextureLoader(const std::filesystem::path& path, AssetContext& ctx)
	{
		// デバイスを取得
		auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// テクスチャを用意
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		HRESULT hr;

		// キーの拡張子を取得
		std::string extension = path.extension().string();

		// ddsなら
		if (extension == ".dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(
				device,                 // ID3D11Device*
				path.wstring().data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}
		// それ以外なら
		else
		{
			hr = DirectX::CreateWICTextureFromFile(
				device,                 // ID3D11Device*
				path.wstring().data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}

		// ロードリザルトを作成
		LoadResult result{};

		// 失敗したら
		if (FAILED(hr))
		{
			// 失敗フラグを立てる
			result.scuccess = false;
		}
		else
		{
			// Textureの生成
			result.asset = std::make_unique<Texture>(std::move(texture));
		}

		result.scuccess = true;

		return result;
	}

	LoadResult Loader::FontLoader(const std::filesystem::path& path, AssetContext& ctx)
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

		// リザルトの作成
		LoadResult loadResult{};

		loadResult.scuccess = true;

		// 返す
		loadResult.asset = std::make_unique<Font>(std::move(font), height);

		return loadResult;
	}

	LoadResult Loader::PrefabLoader(const std::filesystem::path& path, AssetContext& ctx)
	{
		LoadResult result{};

		result.asset = std::make_unique<Prefab>(std::filesystem::path(path));

		result.scuccess = true;

		return result;
	}

	LoadResult Loader::AudioClipLoader(const std::filesystem::path& path, AssetContext& ctx)
	{
		LoadResult result{};

		// AudioEngineを取得
		auto ae = ResourceManager::Instance().GetAudioEngine();

		// 生成
		auto sound = std::make_unique<DirectX::SoundEffect>(ae, path.c_str());

		result.asset = std::make_unique<AudioClip>(std::move(sound));

		result.scuccess = true;

		// 返す
		return result;
	}
}
