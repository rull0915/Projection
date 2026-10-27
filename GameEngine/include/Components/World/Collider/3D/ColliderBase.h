/**
 * @file    ColliderBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/18
 *
 * @brief   衝突判定の基底クラス
 */

#pragma once

#include <SimpleMath.h>

#include "../ColliderCommon.h"
#include "Math/Bounding.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief 3Dコライダーの基底
	 */
	class ColliderBase : public ColliderCommon
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderCommon);

	private:

		/** @brief ローカル座標系での中心座標 */
		DirectX::SimpleMath::Vector3 m_localCenterPos;

		/** @brief 自身を覆うBoundingBox */
		mutable BoundingBox m_boundingBox;

		/** @brief ワールド座標系での中心座標のキャッシュ */
		mutable DirectX::SimpleMath::Vector3 m_worldCenterPos;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		ColliderBase(IComponentOwner* own)
			: ColliderCommon(own)
			, m_boundingBox{ {0, 0, 0}, {0, 0, 0} }
			, m_localCenterPos{ 0, 0, 0 }
		{
			ADD_PROPERTY(ColliderBase, m_localCenterPos);
		}

		virtual ~ColliderBase() = default;

		/// @}

		/** @brief キャッシュを更新する */
		virtual void UpdateCache() const = 0;

		/** @brief GUI上での値変更時にDirtyにする */
		void OnValidate() override
		{
			SetDirty();
		}

		/// @name ゲッター
		/// @{

		/** @brief ローカル座標系での中心座標 */
		DirectX::SimpleMath::Vector3 GetLocalCenterPos() const { return m_localCenterPos; }

		/** @brief BoundingBox */
		inline BoundingBox& GetBoundingBox() const { return m_boundingBox; }

		/** @brief ワールド座標系での中心座標 */
		DirectX::SimpleMath::Vector3 GetWorldCenterPos() const
		{
			if (IsDirty()) UpdateCache();
			return m_worldCenterPos;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief ローカル座標系での中心座標 */
		void SetLocalPos(DirectX::SimpleMath::Vector3 pos)
		{
			m_localCenterPos = pos;
			SetDirty();
		}

		/// @}

	protected:

		/**
		 * @brief ワールド座標系での中心座標を更新する
		 * @param pos 計算済み座標
		 */
		inline void SetWorldPosition(const DirectX::SimpleMath::Vector3& pos) const { m_worldCenterPos = pos; }

		/**
		 * @brief BoundingBoxを更新する
		 * @param box 計算済みBoundingBox
		 */
		inline void SetBoundingBox(const BoundingBox& box) const { m_boundingBox = box; }
	};
} // namespace RHEngine
