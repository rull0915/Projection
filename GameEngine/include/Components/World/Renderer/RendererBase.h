/**
 * @file    RendererBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/28
 *
 * @brief   レンダラーコンポーネントの基底クラスです
 */

#pragma once

#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 描画を行うコンポーネントの基底
	 */
	class RendererBase : public WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);

	private:

		/** @brief 自身のトランスフォームポインタ */
		Transform* m_pTransform;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		RendererBase(IComponentOwner* pOwner);

		~RendererBase() = default;

		/// @}

		/**
		 * @brief 描画を行う
		 */
		virtual void Draw(Renderer& renderer) = 0;

	protected:
		/// @name ゲッター
		/// @{

		/** @brief トランスフォームポインタ */
		Transform* GetTransform() const { return m_pTransform; }

		/// @}
	};
} // namespace RHEngine
