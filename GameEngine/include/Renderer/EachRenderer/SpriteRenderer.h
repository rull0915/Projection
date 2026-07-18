//====================================================//
// ファイル名   : SpriteRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : スプライトの描画を担当するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <SpriteBatch.h>

#include "Renderer/RenderStateCache.h"
#include "IRenderer.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Renderer;

	//====================================================//
	// クラス宣言
	//====================================================//
	class SpriteRenderer : public IRenderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 所有者のRenderer
		Renderer& m_renderer;

		// 描画の状態
		RenderStateCache& m_renderState;

		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SpriteRenderer(Renderer& renderer);
		~SpriteRenderer();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Initialize() override;
		void Start() override;
		void End() override;

		//-----------------------------------------------------
		// 描画関数
		//-----------------------------------------------------

		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 pos,
			RECT* srcRect,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		)
		{
			if (texture) m_spriteBatch->Draw(texture, pos, srcRect, color, angle, origin, scale);
		}

		static DirectX::SimpleMath::Vector2 GetTextureSize(ID3D11ShaderResourceView* srv)
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
	};
}	// namespace REngine
