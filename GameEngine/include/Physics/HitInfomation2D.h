/**
 * @file    HitInfomation2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 *
 * @brief   2D版衝突情報構造体
 */

#pragma once

#include <SimpleMath.h>

#include "GameObject/Interface/IComponentOwner.h"

namespace RHEngine
{
	/**
	 * @brief 2D衝突情報構造体
	 */
	struct HitInfomation2D
	{
		DirectX::SimpleMath::Vector2 hitDir = DirectX::SimpleMath::Vector2::Zero;	///< 押し戻し方向（相手から自分へ向かうベクトル）
		float hitLen = 0.0f;				///< 押し戻し距離（めり込み深さ）
		IComponentOwner* own = nullptr;		///< 自分自身
		IComponentOwner* target = nullptr;	///< 衝突相手

		/**
		 * @brief 相手視点の衝突情報を作成する
		 * @return 作成した衝突情報
		 */
		HitInfomation2D Inverse() const {
			HitInfomation2D inv;
			inv.hitDir = -this->hitDir; // 方向を反転
			inv.hitLen = this->hitLen;  // 距離は不変
			inv.own = this->target;     // 自分のターゲットを、新しい「自分」に
			inv.target = this->own;     // 自分自身を、新しい「ターゲット」に
			return inv;
		}
	};
}	// namespace RHEngine
