//====================================================//
// ファイル名   : CheckHit.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要 : 衝突判定関数群
//
// 更新履歴 :
// 2026/06/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Collider/2D/ColliderBase2D.h"

#include "Physics/HitInfomation2D.h"

//====================================================//
// クラス宣言
//====================================================//

namespace Collision2D
{
	/// <summary>
	/// 凸多角形同士
	/// </summary>
	bool CheckHit2D_Convex_Convex(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// 円と凸多角形
	/// </summary>
	bool CheckHit2D_Circle_Convex(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// カプセルと凸多角形
	/// </summary>
	bool CheckHit2D_Capsule_Convex(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);
}
