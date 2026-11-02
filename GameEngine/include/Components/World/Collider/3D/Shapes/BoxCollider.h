/**
 * @file    BoxCollider.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/18
 *
 * @brief   ボックスのコライダー (OBB)
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
	 * @brief 3Dボックスコライダー
	 */
	class BoxCollider : public ColliderBase, public IDebugRenderable
	{
	public:
		DECLARE_COMPONENT_TYPE(ColliderBase);

	private:

		/**
		 * @brief 計算済み情報のキャッシュ
		 */
		struct WorldCache {
			DirectX::SimpleMath::Vector3 xAxis;	///< X軸ベクトル
			DirectX::SimpleMath::Vector3 yAxis;	///< Y軸ベクトル
			DirectX::SimpleMath::Vector3 zAxis;	///< Z軸ベクトル

			DirectX::SimpleMath::Vector3 scale;	///< スケール

			DirectX::SimpleMath::Matrix localMatrix;	///< ワールド行列
			DirectX::SimpleMath::Matrix localMatrixInverse;	///< ワールド逆行列
		};

		/** @brief ローカル座標系でのサイズ */
		DirectX::SimpleMath::Vector3 m_localSize;

		/** @brief 計算済み情報のキャッシュ */
		mutable WorldCache m_cache;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		BoxCollider(IComponentOwner* pOwner);

		~BoxCollider() = default;

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

		/** @brief ローカル座標系でのサイズ */
		DirectX::SimpleMath::Vector3 GetLocalSize() const { return m_localSize; }

		/** @brief X軸ベクトル */
		DirectX::SimpleMath::Vector3 GetXAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.xAxis;
		}

		/** @brief Y軸ベクトル */
		DirectX::SimpleMath::Vector3 GetYAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.yAxis;
		}

		/** @brief Z軸ベクトル */
		DirectX::SimpleMath::Vector3 GetZAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.zAxis;
		}

		/** @brief ワールド座標系での半分のサイズ */
		DirectX::SimpleMath::Vector3 GetHalfSize() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.scale * 0.5f;
		}

		/** @brief ワールド行列 */
		DirectX::SimpleMath::Matrix GetLocalMatrix() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.localMatrix;
		}

		/** @brief ワールド逆行列 */
		DirectX::SimpleMath::Matrix GetLocalMatrixInverse() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.localMatrixInverse;
		}

		/// @}

		/// @name
		/// @{

		/** @brief ローカル座標系でのサイズ */
		void SetLocalSize(DirectX::SimpleMath::Vector3 scale)
		{
			m_localSize = scale;
			SetDirty();
		}

		/// @}
	};
} // namespace RHEngine
