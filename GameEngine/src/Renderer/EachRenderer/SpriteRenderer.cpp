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
	SpriteRenderer::SpriteRenderer(DrawCommandContainer& container, GraphicsSystem& system)
		: m_container{ container }
		, m_graphicSystem{ system }
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	SpriteRenderer::~SpriteRenderer()
	{}

	void SpriteRenderer::Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// コマンドの追加
		auto& command = m_container.AddSprite();

		command.pTexture = texture;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void SpriteRenderer::Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(texture);

		// スケールの計算
		DirectX::SimpleMath::Vector2 finalScale = (max - min) / texSize;

		// 指定の拡大率を乗算する
		finalScale *= scale;

		// コマンドの追加
		auto& command = m_container.AddSprite();

		command.pTexture = texture;
		command.pos = min;
		command.scale = finalScale;
		command.angle = angle;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void SpriteRenderer::Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(texture);

		// 原点を算出
		DirectX::SimpleMath::Vector2 finalOrigin = origin * texSize;

		// コマンドの追加
		auto& command = m_container.AddSprite();

		command.pTexture = texture;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void SpriteRenderer::Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(texture);

		// Originを考慮して中心を計算
		DirectX::SimpleMath::Vector2 center = min + (max - min) * origin;

		// 原点を算出
		DirectX::SimpleMath::Vector2 finalOrigin = origin * texSize;

		// スケールの計算
		DirectX::SimpleMath::Vector2 finalScale = (max - min) / texSize;

		// 指定の拡大率を乗算する
		finalScale *= scale;

		// コマンドの追加
		auto& command = m_container.AddSprite();

		command.pTexture = texture;
		command.pos = center;
		command.scale = finalScale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void SpriteRenderer::Draw(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 pos, RECT* srcRect, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// コマンドの追加
		auto& command = m_container.AddSprite();

		command.pTexture = texture;
		command.pos = pos;
		command.srcRect = srcRect ? std::make_optional(*srcRect) : std::nullopt;
		command.scale = scale;
		command.angle = angle;
		command.origin = origin;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

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
