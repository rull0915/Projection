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

		// デフォルト
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);

		// 描画領域指定
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);

		// 原点指定
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		// 描画領域 原点指定
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		static DirectX::SimpleMath::Vector2 GetTextureSize(DirectX::SpriteFont* font, const wchar_t* text)
		{
			DirectX::XMVECTOR result = font->MeasureString(text);

			return { DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result) };
		}
	};
}	// namespace REngine
