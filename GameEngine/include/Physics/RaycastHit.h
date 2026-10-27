/**
 * @file    RaycastHit.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/31
 *
 * @brief   レイの衝突情報構造体
 */

#pragma once

#include <SimpleMath.h>
#include <cfloat>

namespace RHEngine
{
	class ColliderBase;

	/**
	 * @brief レイの衝突情報
	 */
	struct RaycastHit
	{
		ColliderBase* collider = nullptr;   ///< 衝突したコライダ

		DirectX::SimpleMath::Vector3 point{};	///< 衝突点
		DirectX::SimpleMath::Vector3 normal{};	///< 衝突法線

		float distance = FLT_MAX;	///< 距離
	};
}	// namespace RHEngine
