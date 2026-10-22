//====================================================//
// ファイル名  : DefaultAssetManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/17
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DefaultAssetManager.h"

#include "Assets/Managers/AssetManager.h"
#include "Assets/EngineDefaultAssets.h"

#include "System/Render/GraphicsManager.h"
#include <WICTextureLoader.h> 

// 埋め込みアセットファイルの読み込み
#include "Assets/Embedded/DefaultTexture.h"
#include "Assets/Embedded/RHEngine_Logo.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	void DefaultAssetManager::CreateAndRegister()
	{
		AssetRegistry& registry = m_assetContext.GetRegistry();

		// デフォルトテクスチャの追加登録
		auto handle = registry.Register(DefaultAsset::DEFAULT_SPRITE);
		registry.Replace(handle.index, CreateTextureAsset(AssetData::g_DefaultTexture_data, AssetData::g_DefaultTexture_size));

		// ロゴの追加登録
		handle = registry.Register(DefaultAsset::ENGINE_LOGO_SPRITE);
		registry.Replace(handle.index, CreateTextureAsset(AssetData::g_RHEngine_Logo_data, AssetData::g_RHEngine_Logo_size));
	}

	std::unique_ptr<Texture> DefaultAssetManager::CreateTextureAsset(const unsigned char* data, const unsigned int size)
	{
		// contextを取得
		auto dr = GraphicsManager::Instance().GetDeviceResources();
		auto device = dr->GetD3DDevice();
		auto ctx = dr->GetD3DDeviceContext();

		// SRVを用意
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv{};

		// SRVを生成
		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			device,		// ID3D11Device*
			data,		// メモリ上のバイト配列
			size,		// データサイズ
			nullptr,	// ID3D11Resource**
			srv.GetAddressOf()	// 受け取る SRV ポインタ
		);

		return std::make_unique<Texture>(srv);
	}
}
