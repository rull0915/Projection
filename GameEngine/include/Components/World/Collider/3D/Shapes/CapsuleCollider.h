/**
 * @file    CapsuleCollider.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/18
 *
 * @brief   カプセルのコライダー
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
	 * @brief カプセルコライダー
	 */
	class CapsuleCollider : public ColliderBase, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase);

		/**
		 * @brief 基準とする軸
		 */
		enum class AxisType
		{
			X,
			Y,
			Z,
		};
		
		/**
		 * @brief 計算済みワールド座標系での情報のキャッシュ
		 */
		struct WorldCache {
			float radius;	///< 半径
			float height;	///< カプセル全体の長さ
			float lineLength;	///< 中心線分の長さ
			DirectX::SimpleMath::Vector3 dir;	///< 中心線分の方向ベクトル
			std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> points;	///< 上下の半球の中心点
		};

	private:

		/** @brief 基準とする軸 */
		AxisType m_axis;

		/** @brief カプセル全体の高さ */
		float m_capsuleHeight;

		/** @brief 半径 */
		float m_radius;

		/** @brief ワールド座標系情報のキャッシュ */
		mutable WorldCache m_cache;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		CapsuleCollider(IComponentOwner* pOwner);

		~CapsuleCollider() = default;

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

		/** @brief 基準とする軸 */
		AxisType GetLineAxis() const { return m_axis; }

		/** @brief ローカル座標系での半径 */
		float GetLocalRadius() const { return m_radius;	}

		/** @brief ワールド座標系での半径 */
		float GetRadius() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.radius;
		}

		/** @brief 中心線分の方向ベクトル */
		DirectX::SimpleMath::Vector3 GetLineDir() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.dir;
		}

		/** @brief カプセル全体の高さ */
		float GetHeight() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.height;
		}

		/** @brief 中心線分の長さ */
		float GetLineLength() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.lineLength;
		}

		/** @brief 上下の半球の中心点 */
		std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> GetPoints() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.points;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief カプセル全体の高さ */
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
		void SetAxis(AxisType type)
		{
			m_axis = type;
			SetDirty();
		}

		/// @}
	};
} // namespace RHEngine
