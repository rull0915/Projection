/**
 * @file    Bounding.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/07
 *
 * @brief   境界形状をまとめたヘッダ
 */

#pragma once

#include <SimpleMath.h>

namespace RHEngine
{
	/**
	 * @brief 境界ボックス
	 */
	struct BoundingBox
	{
		DirectX::SimpleMath::Vector3 min;	///< 最小座標
		DirectX::SimpleMath::Vector3 max;	///< 最大座標

		/// @name ライフサイクル
		/// @{

		/**
		 * @param a 1つの点
		 * @param b もう片方の点
		 */
		BoundingBox(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b)
			: min{ a }
			, max{ b }
		{};

		BoundingBox()
			: min{ 0, 0, 0 }, max{ 0, 0, 0 }
		{}

		/// @}
	};

	/**
	 * @brief 2D境界ボックス
	 */
	struct BoundingBox2D
	{
		DirectX::SimpleMath::Vector2 min;	///< 最小座標
		DirectX::SimpleMath::Vector2 max;	///< 最大座標

		/// @name ライフサイクル
		/// @{

		/**
		 * @param a 1つの点
		 * @param b もう一つの点
		 */
		BoundingBox2D(DirectX::SimpleMath::Vector2 a, DirectX::SimpleMath::Vector2 b)
			: min{ a }
			, max{ b }
		{};

		BoundingBox2D()
			: min{ 0, 0 }, max{ 0, 0 }
		{}

		/// @}
	};

	/**
	 * @brief 境界球
	 */
	struct BoundingSphere
	{
		DirectX::SimpleMath::Vector3 center;	///< 中心座標
		float radius;							///< 半径

		/// @name ライフサイクル
		/// @{

		/**
		 * @param c 中心座標
		 * @param r 半径
		 */
		BoundingSphere(DirectX::SimpleMath::Vector3 c, float r)
			: center{ c }
			, radius{ r }
		{};

		BoundingSphere()
			: center{ 0, 0, 0 }, radius{ 0 }
		{}

		/// @}
	};
	
	/**
	 * @brief 境界円
	 */
	struct BoundingCircle
	{
		DirectX::SimpleMath::Vector2 center;	///< 中心座標
		float radius;							///< 半径

		/// @name ライフサイクル
		/// @{

		/**
		 * @param c 中心座標
		 * @param r 半径
		 */
		BoundingCircle(DirectX::SimpleMath::Vector2 c, float r)
			: center{ c }
			, radius{ r }
		{};

		BoundingCircle()
			: center{ 0, 0 }, radius{ 0 }
		{}

		/// @}
	};
}	// namespace RHEngine
