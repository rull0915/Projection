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

#include "Command/Publisher/DrawPrimitiveCommandPublisher.h"
#include "Command/Publisher/DrawMeshCommandPublisher.h"
#include "Command/Publisher/DrawSpriteCommandPublisher.h"
#include "Command/Publisher/DrawTextCommandPublisher.h"
#include "Command/Publisher/DrawUICommandPublisher.h"

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
		std::unique_ptr<DrawPrimitiveCommandPublisher> m_primitiveRenderer;

		// メッシュ
		std::unique_ptr<DrawMeshCommandPublisher> m_meshRenderer;

		// スプライト
		std::unique_ptr<DrawSpriteCommandPublisher> m_spriteRenderer;

		// 文字列
		std::unique_ptr<DrawTextCommandPublisher> m_textRenderer;

		// UI
		std::unique_ptr<DrawUICommandPublisher> m_uiRenderer;

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
		DrawPrimitiveCommandPublisher& Primitive() { return *m_primitiveRenderer; }

		// Model
		DrawMeshCommandPublisher& Mesh() { return *m_meshRenderer; }

		// Sprite
		DrawSpriteCommandPublisher& Sprite() { return *m_spriteRenderer; }

		// Text
		DrawTextCommandPublisher& Text() { return *m_textRenderer; }

		// UI
		DrawUICommandPublisher& UI() { return *m_uiRenderer; }
	};
}
