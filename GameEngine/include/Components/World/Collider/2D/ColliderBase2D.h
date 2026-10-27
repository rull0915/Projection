/**
 * @file    ColliderBase2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 *
 * @brief   2Dコライダー基底クラス
 */

#pragma once

#include <SimpleMath.h>

#include "../ColliderCommon.h"
#include "Math/Bounding.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief 2Dコライダーの基底
	 */
	class ColliderBase2D : public ColliderCommon
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderCommon);

	private:

		/** @brief ローカル中心座標 */
		DirectX::SimpleMath::Vector2 m_localCenterPos;

		/** @brief 回転角度 */
		float m_rotation;

		/** @brief 自身を覆うBoundingBox */
		mutable BoundingBox2D m_boundingBox;

		/** @brief ワールド中心座標のキャッシュ */
		mutable DirectX::SimpleMath::Vector2 m_worldCenterPos;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		ColliderBase2D(IComponentOwner* pOwner)
			: ColliderCommon(pOwner)
			, m_localCenterPos{ 0, 0 }
			, m_rotation{ 0.0f }
			, m_boundingBox{}
			, m_worldCenterPos{}
		{
			ADD_PROPERTY(ColliderBase2D, m_localCenterPos);
			ADD_PROPERTY(ColliderBase2D, m_rotation);
		}

		virtual ~ColliderBase2D() = default;

		/// @}

		/**
		 * @brief キャッシュを更新する
		 */
		virtual void UpdateCache() const = 0;

		/**
		 * @brief GUI上での値変更時にDirtyにする
		 */
		void OnValidate() override
		{
			SetDirty();
		}

		/// @name ゲッター
		/// @{
		
		/** @brief BoundingBox */
		inline BoundingBox2D& GetBoundingBox() const { return m_boundingBox; }

		/** @brief ワールド座標系の中心座標 */
		DirectX::SimpleMath::Vector2 GetWorldCenterPos() const
		{
			if (IsDirty()) UpdateCache();
			return m_worldCenterPos;
		}
		
		/** @brief ローカル座標系での中心座標 */
		DirectX::SimpleMath::Vector2 GetLocalCenterPos() const { return m_localCenterPos; }

		/** @brief 回転角度 (ラジアン) */
		float GetRotation() const { return m_rotation; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief ローカル中心座標 */
		void SetLocalPos(DirectX::SimpleMath::Vector2 pos)
		{
			m_localCenterPos = pos;
			SetDirty();
		}

		/** @brief 回転角度 */
		void SetRotation(float rot)
		{
			m_rotation = rot;
			SetDirty();
		}
	protected:

		/**
		 * @brief ワールド座標系中心座標のキャッシュを更新する
		 * @param pos 計算済みワールド座標系中心座標
		 */
		inline void SetWorldPosition(const DirectX::SimpleMath::Vector2& pos) const { m_worldCenterPos = pos; }

		/**
		 * @brief BoundingBoxを更新する
		 * @param box 計算済みBoundingBox
		 */
		inline void SetBoundingBox(const BoundingBox2D& box) const { m_boundingBox = box; }
	};
} // namespace RHEngine
