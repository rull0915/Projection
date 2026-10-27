/**
 * @file    CapsuleCollider2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/18
 *
 * @brief   2Dカプセルコライダー
 */

#pragma once

#include <utility>
#include <SimpleMath.h>

#include "../ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;

	/**
	 * @brief 2Dカプセルのコライダー
	 */
	class CapsuleCollider2D : public ColliderBase2D, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase2D);

		/**
		 * @brief 基準とする軸
		 */
		enum class AxisType2D
		{
			Horizontal,	///< X軸
			Vertical,	///< Y軸
		};

	private:
		/**
		 * @brief 計算済み情報のキャッシュ
		 */
		struct WorldCache {
			float lineLength;					///< 中心円柱の高さ
			DirectX::SimpleMath::Vector2 dir;	///< ワールド座標系での中心線分の方向ベクトル
			std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> points;	///< 上下の半球の中心点
		};

	private:

		/** @brief 基準とする軸 */
		AxisType2D m_axisType;

		/**
		 * @brief カプセルの高さ
		 * @details 上下の半球含めた全体の高さ
		 */
		float m_capsuleHeight;

		/** @brief 半径 */
		float m_radius;

		/** @brief 計算済み情報のキャッシュ */
		mutable WorldCache m_cache;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		CapsuleCollider2D(IComponentOwner* pOwner);
			
		~CapsuleCollider2D() = default;

		/// @}

		/**
		 * @brief キャッシュを更新する
		 */
		void UpdateCache() const override;

		/**
		 * @brief 衝突形状のデバッグ描画を行う
		 */
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color);

		/// @name ゲッター
		/// @{

		/** @brief 半径 */
		float GetRadius() const { return m_radius; }

		/** @brief カプセル全体の高さ */
		float GetHeight() const { return m_capsuleHeight; }

		/** @brief カプセルの基準軸 */
		AxisType2D GetAxis() const { return m_axisType; }

		/** @brief 中心線分の方向ベクトル */
		DirectX::SimpleMath::Vector2 GetLineDir() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.dir;
		}

		/** @brief 中心線分の長さ */
		float GetLineLength() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.lineLength;
		}

		/** @brief 上下半球の中心点 */
		std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> GetPoints() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.points;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief カプセルの高さ */
		void SetHeight(float height)
		{
			m_capsuleHeight = height;
			SetDirty();
		}

		/** @brief 半径 */
		void SetRadius(float radius)
		{
			m_radius = radius;
			SetDirty();
		}

		/** @brief 基準軸 */
		void SetAxis(AxisType2D type)
		{
			m_axisType = type;
			SetDirty();
		}
	};
} // namespace RHEngine
