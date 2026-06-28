//====================================================//
// ファイル名   : CheckHit.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : コライダーの衝突判定をまとめたヘッダー
//
// 更新履歴 :
// 2026/03/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "Components/World/Collider/3D/Shapes/Colliders.h"
#include "Physics/HitInfomation.h"

//====================================================//
// 関数宣言
//====================================================//

namespace Collision
{
	/// <summary>
	/// 球同士の衝突判定
	/// </summary>
	bool CheckHit_Sphere_Sphere(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

	/// <summary>
	/// 球とカプセル
	/// </summary>
	bool CheckHit_Sphere_Capsule(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

	/// <summary>
	/// 球とボックス
	/// </summary>
	bool CheckHit_Sphere_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

	/// <summary>
	/// カプセル同士
	/// </summary>
	bool CheckHit_Capsule_Capsule(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

	/// <summary>
	/// カプセルとボックス
	/// </summary>
	bool CheckHit_Capsule_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);

	/// <summary>
	/// ボックス同士
	/// </summary>
	bool CheckHit_Box_Box(ColliderBase* colA, ColliderBase* colB, HitInfomation* info = nullptr);
}
