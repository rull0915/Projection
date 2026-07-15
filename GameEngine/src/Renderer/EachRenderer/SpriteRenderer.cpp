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
#include "Renderer/Renderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpriteRenderer::SpriteRenderer(Renderer& renderer)
		: m_renderer{ renderer }
		, m_renderState{ renderer.GetRenderState() }
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	SpriteRenderer::~SpriteRenderer()
	{}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void SpriteRenderer::Initialize()
	{
		auto context = m_renderer.GetContext();

		// スプライトバッチの初期化
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	}

	/// <summary>
	/// 描画開始関数
	/// </summary>
	void SpriteRenderer::Start()
	{
		m_spriteBatch->Begin(
			DirectX::SpriteSortMode_Deferred,
			m_renderer.GetStates()->NonPremultiplied()
		);
	}

	/// <summary>
	/// 描画終了関数
	/// </summary>
	void SpriteRenderer::End()
	{
		m_spriteBatch->End();
	}
}	// namespace REngine
