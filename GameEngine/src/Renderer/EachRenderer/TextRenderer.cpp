//====================================================//
// ファイル名  : TextRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/01
//
// 概要        :  テキストの描画を担当する関数
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/TextRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextRenderer::TextRenderer(DrawCommandContainer& container)
		: m_container{ container }
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	TextRenderer::~TextRenderer()
	{}
}	// namespace REngine
