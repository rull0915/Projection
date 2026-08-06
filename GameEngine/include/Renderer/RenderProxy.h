//====================================================//
// ファイル名   : RenderProxy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画仲介クラス
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>

#include "EachRenderer/PrimitiveRenderer.h"
#include "EachRenderer/SpriteRenderer.h"
#include "EachRenderer/ModelRenderer.h"
#include "EachRenderer/TextRenderer.h"
#include "EachRenderer/UIRenderer.h"

namespace REngine
{
	class DrawCommandContainer;
	class GraphicsSystem;

	//====================================================//
	// クラス宣言
	//====================================================//
	class RenderProxy
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// プリミティブ
		std::unique_ptr<PrimitiveRenderer> m_primitiveRenderer;

		// モデル
		std::unique_ptr<ModelRenderer> m_modelRenderer;

		// スプライト
		std::unique_ptr<SpriteRenderer> m_spriteRenderer;

		// 文字列
		std::unique_ptr<TextRenderer> m_textRenderer;

		// UI
		std::unique_ptr<UIRenderer> m_uiRenderer;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		RenderProxy() = default;
		~RenderProxy() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize(GraphicsSystem& system, DrawCommandContainer& container);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// Primitive
		PrimitiveRenderer& Primitive() { return *m_primitiveRenderer; }

		// Model
		ModelRenderer& Model() { return *m_modelRenderer; }

		// Sprite
		SpriteRenderer& Sprite() { return *m_spriteRenderer; }

		// Text
		TextRenderer& Text() { return *m_textRenderer; }

		// UI
		UIRenderer& UI() { return *m_uiRenderer; }
	};
}
