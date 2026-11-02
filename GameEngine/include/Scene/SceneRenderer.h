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

	/**
	 * @brief シーンの描画担当
	 */
	class SceneRenderer
	{
	public:
		/** @brief デバッグ描画時の描画色 */
		static constexpr DirectX::SimpleMath::Color DEBUG_COLOR = { 0, 1, 0, 1 };

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
	};
}	// namespace RHEngine
