/**
 * @file    StandardCheckHit2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 *
 * @brief   2Dコライダーの衝突判定をまとめたヘッダー
 */

#pragma once

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "Physics/HitInfomation2D.h"

namespace RHEngine
{
	namespace Collision2D
	{
		/**
		 * @brief 円同士の衝突判定
		 */
		bool CheckHit_Circle_Circle(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

		/**
		 * @brief 円とカプセルの衝突判定
		 */
		bool CheckHit_Circle_Capsule(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

		/**
		 * @brief 円とボックス衝突判定
		 */
		bool CheckHit_Circle_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

		/**
		 * @brief カプセル同士の衝突判定
		 */
		bool CheckHit_Capsule_Capsule(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

		/**
		 * @brief カプセルとボックスの衝突判定
		 */
		bool CheckHit_Capsule_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

		/**
		 * @brief ボックス同士の衝突判定
		 */
		bool CheckHit_Box_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);
	}
}	// namespace RHEngine
