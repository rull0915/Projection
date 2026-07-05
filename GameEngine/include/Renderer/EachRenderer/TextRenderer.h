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
#include <SpriteFont.h>

#include "Renderer/RenderStateCache.h"
#include "IRenderer.h"

//====================================================//
// 前方宣言
//====================================================//
class Renderer;

//====================================================//
// クラス宣言
//====================================================//
class TextRenderer : public IRenderer
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 所有者のRenderer
	Renderer& m_renderer;

	// 描画の状態
	RenderStateCache& m_renderState;

	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	TextRenderer(Renderer& renderer);
	~TextRenderer();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Initialize() override;
	void Start() override;
	void End() override;

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
		color.w = m_renderState.GetAlpha();

		if (spriteFont) spriteFont->DrawString(
			m_spriteBatch.get(),
			text,
			pos,
			color,
			angle,
			origin,
			scale);
	}

	static DirectX::SimpleMath::Vector2 GetTextureSize(DirectX::SpriteFont* font, const wchar_t* text)
	{
		DirectX::XMVECTOR result = font->MeasureString(text);

		return { DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result) };
	}
};
