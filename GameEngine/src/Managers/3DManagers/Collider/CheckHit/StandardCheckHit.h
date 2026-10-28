/**
 * @file    CheckHit.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/18
 *
 * @brief   コライダーの衝突判定をまとめたヘッダー
 */

#pragma once

#include "Components/World/Collider/3D/Shapes/Colliders.h"
#include "Physics/HitInfomation.h"

namespace RHEngine
{
	namespace Collision
	{
		/**
		 * @brief 球同士の衝突判定
		 */
		bool CheckHit_Sphere_Sphere(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

		/**
		 * @brief 球とカプセル衝突判定
		 */
		bool CheckHit_Sphere_Capsule(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

		/**
		 * @brief 球とボックス衝突判定
		 */
		bool CheckHit_Sphere_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

		/**
		 * @brief カプセル同士の衝突判定
		 */
		bool CheckHit_Capsule_Capsule(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

		/**
		 * @brief カプセルとボックスの衝突判定
		 */
		bool CheckHit_Capsule_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);
		
		/**
		 * @brief ボックス同士の衝突判定
		 */
		bool CheckHit_Box_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);
	}
}	// namespace RHEngine
