//====================================================//
// ファイル名  : GameInitializer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameInitializer.h"

#include "Managers/CollisionMap.h"
#include "Debug/ColliderDebugRenderer.h"

#include "GameLib/Colliders/CheckHit.h"
#include "GameLib/Colliders/DebugRenderer.h"
#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"

#include "System/TypeIdGenerator.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void GameInitializer::Initialize()
{
	// 衝突判定関数の追加
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<ConvexPolygonCollider2D>(), TypeIDGenerator::GetID<ConvexPolygonCollider2D>(),
		Collision2D::CheckHit2D_Convex_Convex
	);
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(), TypeIDGenerator::GetID<ConvexPolygonCollider2D>(),
		Collision2D::CheckHit2D_Circle_Convex
	);
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(), TypeIDGenerator::GetID<ConvexPolygonCollider2D>(),
		Collision2D::CheckHit2D_Capsule_Convex
	);

	// デバッグ描画関数の追加
	ColliderDebugRenderer2D::Register(
		TypeIDGenerator::GetID<ConvexPolygonCollider2D>(),
		DrawCollider2D::DrawCollider_Convex
	);

	// 作成関数の追加
}
