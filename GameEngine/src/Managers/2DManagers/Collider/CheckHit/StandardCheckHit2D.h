//====================================================//
// ファイル名   : StandardCheckHit2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : 2Dコライダーの衝突判定をまとめたヘッダー
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "Physics/HitInfomation2D.h"

//====================================================//
// 関数宣言
//====================================================//

namespace Collision2D
{
	/// <summary>
	/// 円同士の衝突判定
	/// </summary>
	bool CheckHit_Circle_Circle(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// 円とカプセル
	/// </summary>
	bool CheckHit_Circle_Capsule(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// 円とボックス
	/// </summary>
	bool CheckHit_Circle_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// カプセル同士
	/// </summary>
	bool CheckHit_Capsule_Capsule(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// カプセルとボックス
	/// </summary>
	bool CheckHit_Capsule_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);

	/// <summary>
	/// ボックス同士
	/// </summary>
	bool CheckHit_Box_Box(ColliderBase2D* colA, ColliderBase2D* colB, HitInfomation2D* info);
}
