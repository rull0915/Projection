/**
 * @file    IDebugRenderable.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/10
 *
 * @brief   デバッグ描画機能を実装するインターフェース
 */

#pragma once

#include <SimpleMath.h>

namespace RHEngine
{
	class Renderer;

	/**
	 * @brief デバッグ描画をするコンポーネントのインターフェース
	 */
	class IDebugRenderable
	{
	public:

		static constexpr DirectX::SimpleMath::Color DEFAULT_DEBUG_COLOR = { 0.9f, 1.0f, 0.9f, 1.0f };

		/// @name ライフサイクル
		/// @{

		IDebugRenderable() = default;

		virtual ~IDebugRenderable() = default;

		/// @}

		/**
		 * @brief デバッグ描画を行う
		 * @param renderer 描画仲介クラス
		 */
		virtual void DebugRender(
			Renderer& renderer
		) = 0;
	};
} // namespace RHEngine
