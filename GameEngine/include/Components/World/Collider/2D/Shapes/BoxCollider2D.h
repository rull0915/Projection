/**
 * @file    BoxCollider2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 *
 * @brief   2Dボックスコライダー
 */

#pragma once

#include <SimpleMath.h>

#include "../ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 2Dボックスコライダー
	 */
	class BoxCollider2D : public ColliderBase2D, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase2D);

	private:

		/**
		 * @brief 計算済み情報のキャッシュ
		 */
		struct WorldCache {
			DirectX::SimpleMath::Vector2 xAxis;	///< x軸ベクトル
			DirectX::SimpleMath::Vector2 yAxis;	///< y軸ベクトル
		};

		/** @brief ローカル座標系でのサイズ */
		DirectX::SimpleMath::Vector2 m_localSize;

		/** @brief 計算済みキャッシュ */
		mutable WorldCache m_cache;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		BoxCollider2D(IComponentOwner* pOwner);

		~BoxCollider2D() = default;

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

		/** @brief X軸ベクトル */
		DirectX::SimpleMath::Vector2 GetXAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.xAxis;
		}

		/** @brief Y軸ベクトル */
		DirectX::SimpleMath::Vector2 GetYAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.yAxis;
		}

		/** @brief サイズの半分 */
		DirectX::SimpleMath::Vector2 GetHalfSize() const
		{
			return m_localSize * 0.5f;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief ローカル座標系でのサイズ */
		void SetLocalSize(DirectX::SimpleMath::Vector2 scale)
		{
			m_localSize = scale;
			SetDirty();
		}

		/// @}
	};
} // namespace RHEngine
