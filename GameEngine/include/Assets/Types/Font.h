//====================================================//
// ファイル名   : Font.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/27
//
// 概要 : フォントアセット
//
// 更新履歴 :
// 2026/07/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <SpriteFont.h>
#include "Assets/Objects/AssetBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Font : public AssetBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ShaderResourceView
		std::unique_ptr<DirectX::SpriteFont> m_font;

		// フォント
		float m_height;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Font(std::unique_ptr<DirectX::SpriteFont> font, float height)
			: m_font{ std::move(font) }
			, m_height{ height }
		{}
		~Font() = default;

		// フォントを取得する関数
		DirectX::SpriteFont* Get() const { return m_font.get(); }

		// フォントサイズを取得
		float GetFontSize() const { return m_height; }
	};
}
