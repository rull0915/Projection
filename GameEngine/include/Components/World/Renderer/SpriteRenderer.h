/**
 * @file    SpriteRenderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/10
 *
 * @brief   ワールド空間に画像を描画する
 */

#pragma once

#include "RendererBase.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 描画を行うコンポーネントの基底
	 */
	class SpriteRenderer : public RendererBase
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		SpriteRenderer(IComponentOwner* pOwner);

		~SpriteRenderer() = default;

		/// @}

		/**
		 * @brief 描画を行う
		 */
		virtual void Draw(Renderer& renderer);
	};
} // namespace RHEngine
