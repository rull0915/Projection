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

// 衝突判定
#include "Managers/CollisionMap.h"
#include "Debug/ColliderDebugRenderer.h"
#include "GameLib/Colliders/CheckHit.h"
#include "GameLib/Colliders/DebugRenderer.h"
#include "Components/World/Collider/2D/Shapes/2DColliders.h"

#include "System/TypeIdGenerator.h"
#include "GameObject/GameObject.h"
#include "Loader/ComponentFactory.h"
#include "Input/InputSystem.h"
#include "Input/KeyInput.h"

// 特有コンポーネント群
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"
#include "GamePlayScene/Camera/ProjectionSmoothCamera.h"
#include "GamePlayScene/Camera/TPSCamera.h"
#include "GamePlayScene/ChangeDimention/ChangeColliderComponent.h"
#include "GamePlayScene/ChangeDimention/DepthCorrection.h"
#include "GamePlayScene/Enemy/Enemy.h"
#include "GamePlayScene/Player/Player.h"
#include "GamePlayScene/Stage/Components/GoalComponent.h"

// 生成関数登録マクロ
#define REGISTER_FACTORY(Name) (ComponentFactory::Register(#Name, Name::SPACE, [](GameObject* o){ return o->AddComponent<Name>(); }))

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
	RegistFactries();

	// キーの追加
	AddKey();
}

// 作成関数をエディタに登録する関数
void GameInitializer::RegistFactries()
{
	// Todo: 新規コンポーネントはここに生成関数の登録を追加してください。
	REGISTER_FACTORY(ProjectionSmoothCamera);
	REGISTER_FACTORY(TPSCamera);
	REGISTER_FACTORY(ConvexPolygonCollider2D);
	REGISTER_FACTORY(ChangeColliderComponent);
	REGISTER_FACTORY(DepthCorrection);
	REGISTER_FACTORY(Enemy);
	REGISTER_FACTORY(Player);
	REGISTER_FACTORY(GoalComponent);
}

void GameInitializer::AddKey()
{
	// Qボタンで次元変更
	Input::Custom::AddButton(
		"ChangeDimention", { Input::Type::Key, Input::State::Down, Input::Key::Code::Q }
	);
}
