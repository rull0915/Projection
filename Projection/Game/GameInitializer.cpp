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
	// 名前とクラス名は一致させてください。
	// 
	//	ComponentFactory::Register(
	//	"XXXComponent", [](GameObject* o) { return o->AddComponent<XXXComponent>(); }
	//);

	ComponentFactory::Register(
		"ProjectionSmoothCamera", [](GameObject* o) { return o->AddComponent<ProjectionSmoothCamera>(); }
	);
	ComponentFactory::Register(
		"TPSCamera", [](GameObject* o) { return o->AddComponent<TPSCamera>(); }
	);
	ComponentFactory::Register(
		"ConvexPolygonCollider2D", [](GameObject* o) { return o->AddComponent<ConvexPolygonCollider2D>(); }
	);
	ComponentFactory::Register(
		"ChangeColliderComponent", [](GameObject* o) { return o->AddComponent<ChangeColliderComponent>(); }
	);
	ComponentFactory::Register(
		"DepthCorrection", [](GameObject* o) { return o->AddComponent<DepthCorrection>(); }
	);
	ComponentFactory::Register(
		"Enemy", [](GameObject* o) { return o->AddComponent<Enemy>(); }
	);
	ComponentFactory::Register(
		"Player", [](GameObject* o) { return o->AddComponent<Player>(); }
	);
	ComponentFactory::Register(
		"GoalComponent", [](GameObject* o) { return o->AddComponent<GoalComponent>(); }
	);
}

void GameInitializer::AddKey()
{
	// Qボタンで次元変更
	Input::Custom::AddButton(
		"ChangeDimention", { Input::Type::Key, Input::State::Down, Input::Key::Code::Q }
	);
}
