//====================================================//
// ファイル名   : TextRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : 文字列の描画を担当するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Renderer/Command/DrawCommandContainer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class TextRenderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// コマンドコンテナ
		DrawCommandContainer& m_container;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		TextRenderer(DrawCommandContainer& container);
		~TextRenderer();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		/// <summary>
		/// 描画関数
		/// </summary>
		/// <param name="spriteFont">スプライトフォント</param>
		/// <param name="text">文字列</param>
		/// <param name="pos">位置</param>
		/// <param name="scale">拡大率</param>
		/// <param name="angle">回転角度</param>
		/// <param name="origin">原点</param>
		/// <param name="color">色</param>
		void Draw
		(
			DirectX::SpriteFont* spriteFont,
			const wchar_t* text,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		)
		{
			// コマンドの追加
			auto& command = m_container.AddText();

			command.spriteFont = spriteFont;
			command.text = text;
			command.pos = pos;
			command.scale = scale;
			command.angle = angle;
			command.origin = origin;
			command.color = color;
		}

		static DirectX::SimpleMath::Vector2 GetTextureSize(DirectX::SpriteFont* font, const wchar_t* text)
		{
			DirectX::XMVECTOR result = font->MeasureString(text);

			return { DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result) };
		}
	};
}	// namespace REngine
