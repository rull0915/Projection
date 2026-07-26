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

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void REngine::AssetInitializer::AssetInitialize(AssetManager& assetManager)
	{
		// Textureの登録
		assetManager.Registry<Texture>("Texture", Loader::TextureLoader, { L".png", L".jpeg", L".bmp", L".tiff", L".gif", L".dds" });
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
}
