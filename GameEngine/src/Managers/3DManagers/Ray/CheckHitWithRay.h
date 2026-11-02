/**
 * @file    CheckHitWithRay.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/31
 *
 * @brief   レイとコライダーの衝突を調べる関数群
 */

#pragma once

#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"
#include "Components/World/Collider/3D/ColliderBase.h"

namespace RHEngine
{
	namespace RayCollision
	{
		/**
		 * @brief Rayと球の衝突判定
		 */
		bool CheckHit_Ray_Sphere(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);

		/**
		 * @brief Rayとカプセルの衝突判定
		 */
		bool CheckHit_Ray_Capsule(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);

		/**
		 * @brief Rayとボックスの衝突判定
		 */
		bool CheckHit_Ray_Box(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);
	}
}	// namespace RHEngine
