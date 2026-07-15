//====================================================//
// ファイル名   : CheckHitWithRay.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/31
//
// 概要 : レイとコライダーの衝突を調べる関数群
//
// 更新履歴 :
// 2026/05/31 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"
#include "Components/World/Collider/3D/ColliderBase.h"

namespace REngine
{
	namespace RayCollision
	{
		// vs球
		bool CheckHit_Ray_Sphere(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);

		// vsカプセル
		bool CheckHit_Ray_Capsule(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);

		// vsボックス
		bool CheckHit_Ray_Box(const Ray& ray, float distance, ColliderBase* col, RaycastHit* hit);
	}
}	// namespace REngine
