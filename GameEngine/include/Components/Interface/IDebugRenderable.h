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

		/// @name ライフサイクル
		/// @{

		IDebugRenderable() = default;

		virtual ~IDebugRenderable() = default;

		/// @}

		/**
		 * @brief デバッグ描画を行う
		 * @param renderer 描画仲介クラス
		 * @param color 描画色
		 */
		virtual void DebugRender(
			Renderer& renderer,
			const DirectX::SimpleMath::Color& color
		) = 0;
	};
} // namespace RHEngine
