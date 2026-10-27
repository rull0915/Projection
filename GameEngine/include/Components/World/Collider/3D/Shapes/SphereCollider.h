/**
 * @file    SphereCollider.h
 * @author  Hoshino Ryunosuke
 * @date    2026/2026/03/18
 *
 * @brief   球状のコライダー
 */

#pragma once

#include <SimpleMath.h>

#include "../ColliderBase.h"
#include "Components/Interface/IDebugRenderable.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 球状のコライダー
	 */
	class SphereCollider : public ColliderBase, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase);

	private:

		/** @brief 半径 */
		float m_radius;

		/** @brief ワールド座標系での半径のキャッシュ */
		mutable float m_worldRadius;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		SphereCollider(IComponentOwner* pOwner);

		~SphereCollider() = default;

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

		/** @brief ローカル座標系での半径 */
		float GetLocalRadius() const
		{
			return m_radius;
		}

		/** @brief ワールド座標系での半径 */
		float GetRadius() const
		{
			if (IsDirty()) UpdateCache();

			return m_worldRadius;
		}

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
