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
#include <optional>
#include "Renderer/Command/DrawCommandContainer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SpriteRenderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画の状態
		DrawCommandContainer& m_container;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SpriteRenderer(DrawCommandContainer& container);
		~SpriteRenderer();

		//-----------------------------------------------------
		// 公開関数
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
			// コマンドの追加
			auto& command = m_container.AddSprite();

			command.pTexture = texture;
			command.pos = pos;
			command.srcRect = srcRect ? std::make_optional(*srcRect) : std::nullopt;
			command.scale = scale;
			command.angle = angle;
			command.origin = origin;
			command.color = color;
		}

		static DirectX::SimpleMath::Vector2 GetTextureSize(ID3D11ShaderResourceView* srv);
	};
}	// namespace REngine
