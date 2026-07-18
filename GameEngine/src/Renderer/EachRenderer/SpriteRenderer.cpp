//====================================================//
// ファイル名  : SpriteRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/01
//
// 概要        :  スプライトの描画を担当する関数
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/SpriteRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpriteRenderer::SpriteRenderer(DrawCommandContainer& container)
		: m_container{ container }
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	SpriteRenderer::~SpriteRenderer()
	{}

	// 画像サイズの取得関数
	DirectX::SimpleMath::Vector2 SpriteRenderer::GetTextureSize(ID3D11ShaderResourceView* srv)
	{
		if (!srv) return DirectX::SimpleMath::Vector2(0, 0);

		// ShaderResourceViewからリソースを取得
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		srv->GetResource(resource.GetAddressOf());

		// リソースをTexture2Dに変換
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		HRESULT hr = resource.As(&texture2D);

		if (SUCCEEDED(hr))
		{
			// テクスチャの詳細情報を取得
			D3D11_TEXTURE2D_DESC desc;
			texture2D->GetDesc(&desc);

			// 幅と高さを float にして返す
			return DirectX::SimpleMath::Vector2(
				static_cast<float>(desc.Width),
				static_cast<float>(desc.Height)
			);
		}

		return DirectX::SimpleMath::Vector2(0, 0);
	}
}	// namespace REngine
