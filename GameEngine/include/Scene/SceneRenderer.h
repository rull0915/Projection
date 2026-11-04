/**
 * @file    SceneRenderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/12
 *
 * @brief   シーンの描画担当クラス
 */

#pragma once

#include <SimpleMath.h>

#include "System/Render/RenderContext.h"

namespace RHEngine
{
	class Renderer;
	class ShadowRenderer;
	class Scene;
	class GameObject;

	/**
	 * @brief シーンの描画担当
	 */
	class SceneRenderer
	{
	private:

		/** @brief シーンポインタ */
		Scene* m_pScene;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene シーンポインタ
		 */
		SceneRenderer(Scene* pScene);

		~SceneRenderer() = default;

		/// @}

		/** @brief ShadowPass描画 */
		void DrawShadowPass(const ShadowRenderContext& ctx, ShadowRenderer& renderer);

		/** @brief WorldPass描画 */
		void DrawWorldPass(const RenderContext& ctx, Renderer& renderer);

		/** @brief UIPass描画 */
		void DrawUIPass(const RenderContext& ctx, Renderer& renderer);

		/**
		 * @brief 1オブジェクトのデバッグ描画をする
		 * @param ctx 使用するコンテキスト
		 * @param pObject 対象のオブジェクト
		 * @param renderer 描画担当
		 */
		void DrawDebug(const RenderContext& ctx, GameObject* pObject, Renderer& renderer);

	private:

		/**
		 * @brief 1オブジェクトのデバッグ描画を再帰的に行う
		 */
		void DrawDebugObject(GameObject* pObject, Renderer& renderer);
	};
}	// namespace RHEngine
