/**
 * @file    RenderProxy.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 *
 * @brief   描画仲介クラス
 */

#pragma once

#include <memory>

#include "Command/Publisher/DrawPrimitiveCommandPublisher.h"
#include "Command/Publisher/DrawMeshCommandPublisher.h"
#include "Command/Publisher/DrawSpriteCommandPublisher.h"
#include "Command/Publisher/DrawTextCommandPublisher.h"
#include "Command/Publisher/DrawUICommandPublisher.h"

namespace RHEngine
{
	class DrawCommandContainer;
	class GraphicsSystem;

	/**
	 * @brief 描画仲介クラス
	 */
	class RenderProxy
	{
	private:

		/** @brief プリミティブ */
		std::unique_ptr<DrawPrimitiveCommandPublisher> m_primitiveRenderer;

		/** @brief メッシュ */
		std::unique_ptr<DrawMeshCommandPublisher> m_meshRenderer;

		/** @brief スプライト */
		std::unique_ptr<DrawSpriteCommandPublisher> m_spriteRenderer;

		/** @brief 文字列 */
		std::unique_ptr<DrawTextCommandPublisher> m_textRenderer;

		/** @brief UI */
		std::unique_ptr<DrawUICommandPublisher> m_uiRenderer;

	public:

		/// @name ライフサイクル
		/// @{

		RenderProxy() = default;

		~RenderProxy() = default;

		/// @}

		/**
		 * @brief 書く描画コマンドパブリッシャーを作成する
		 * @param system 描画システム
		 * @param container コマンドコンテナ
		 */
		void Initialize(GraphicsSystem& system, DrawCommandContainer& container);

		/// @name ゲッター
		/// @{

		/** @brief Primitive */
		DrawPrimitiveCommandPublisher& Primitive() { return *m_primitiveRenderer; }

		/** @brief Model */
		DrawMeshCommandPublisher& Mesh() { return *m_meshRenderer; }

		/** @brief Sprite */
		DrawSpriteCommandPublisher& Sprite() { return *m_spriteRenderer; }

		/** @brief Text */
		DrawTextCommandPublisher& Text() { return *m_textRenderer; }

		/** @brief UI */
		DrawUICommandPublisher& UI() { return *m_uiRenderer; }

		/// @}
	};
}	// namespace RHEngine
