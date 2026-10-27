/**
 * @file    Ray.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/30
 *
 * @brief   レイクラス
 */

#pragma once

#include <SimpleMath.h>

namespace RHEngine
{
	/**
	 * @brief Rayクラス
	 */
	class Ray
	{
	private:

		/** @brief 始点 */
		DirectX::SimpleMath::Vector3 m_start;

		/** @brief 方向ベクトル */
		DirectX::SimpleMath::Vector3 m_direction;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param start 始点
		 * @param direction 方向ベクトル
		 */
		Ray(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 direction)
			: m_start{ start }
			, m_direction{ direction }
		{
			m_direction.Normalize();
		}

		~Ray() = default;

		/// @}

		/// @name ゲッター
		/// @{

		/** @brief 始点 */
		const DirectX::SimpleMath::Vector3& GetStart() const { return m_start; }

		/** @brief 方向ベクトル */
		const DirectX::SimpleMath::Vector3& GetDirection() const { return m_direction; }

		/**
		 * @brief 最小座標を取得する
		 * @param maxDistance Rayの長さ
		 * @return 最小座標
		 */
		const DirectX::SimpleMath::Vector3 GetMin(float maxDistance) const
		{
			return DirectX::SimpleMath::Vector3{
				m_direction.x > 0 ? m_start.x : m_start.x + m_direction.x * maxDistance,
				m_direction.y > 0 ? m_start.y : m_start.y + m_direction.y * maxDistance,
				m_direction.z > 0 ? m_start.z : m_start.z + m_direction.z * maxDistance
			};
		}

		/**
		 * @brief 最大座標を取得する
		 * @param maxDistance Rayの長さ
		 * @return 最大座標
		 */
		const DirectX::SimpleMath::Vector3 GetMax(float maxDistance) const
		{
			return DirectX::SimpleMath::Vector3{
				m_direction.x < 0 ? m_start.x : m_start.x + m_direction.x * maxDistance,
				m_direction.y < 0 ? m_start.y : m_start.y + m_direction.y * maxDistance,
				m_direction.z < 0 ? m_start.z : m_start.z + m_direction.z * maxDistance
			};
		}
	};
}	// namespace RHEngine
