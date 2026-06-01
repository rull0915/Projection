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
#include "Ray.h"
#include "RaycastHit.h"
#include "GameLib/GameObject/Components/Collider/3D/Shapes/Colliders.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

// 基盤となる関数
bool CheckHitWithRay(const Ray& ray, float distance, BaseCollider* col, RaycastHit& hit);

// vs球
bool CheckHitWithRay(const Ray& ray, float distance, SphereCollider* col, RaycastHit& hit);

// vsカプセル
bool CheckHitWithRay(const Ray& ray, float distance, CapsuleCollider* col, RaycastHit& hit);

// vsボックス
bool CheckHitWithRay(const Ray& ray, float distance, BoxCollider* col, RaycastHit& hit);
