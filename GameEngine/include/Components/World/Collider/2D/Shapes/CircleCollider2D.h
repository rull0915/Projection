/**
 * @file    CircleCollider2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 *
 * @brief   2D円コライダー
 */

#pragma once

#include "../ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 2D円コライダー
	 */
	class CircleCollider2D : public ColliderBase2D, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase2D);

	private:

		/** @brief 半径 */
		float m_radius;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		CircleCollider2D(IComponentOwner* pOwner);

		~CircleCollider2D() = default;

		/// @}

		/**
		 * @brief キャッシュを更新する
		 */
		void UpdateCache() const override;

		/**
		 * @brief 衝突形状のデバッグ描画を行う
		 */
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color) override;

		/// @name ゲッター
		/// @{

		/** @brief 半径 */
		float GetRadius() const { return m_radius; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 半径 */
		void SetRadius(float rad)
		{
			m_radius = rad;
			SetDirty();
		}

		/// @}
	};
} // namespace RHEngine
