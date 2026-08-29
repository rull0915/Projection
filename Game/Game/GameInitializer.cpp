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

#include "ComponentRegister.h"

// 衝突判定
#include "Managers/CollisionMap.h"
#include "GameLib/Colliders/CheckHit.h"
#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"

// 入力関連
#include "Common/TypeIdGenerator.h"
#include "Input/InputSystem.h"
#include "Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void GameInitializer::Initialize()
{
	// 衝突判定関数の追加
	REngine::CollisionMap2D::Register(
		ConvexPolygonCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
		Collision2D::CheckHit2D_Convex_Convex
	);
	REngine::CollisionMap2D::Register(
		REngine::CircleCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
		Collision2D::CheckHit2D_Circle_Convex
	);
	REngine::CollisionMap2D::Register(
		REngine::CapsuleCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
		Collision2D::CheckHit2D_Capsule_Convex
	);

	// キーの追加
	AddKey();

	// ファクトリの追加
	ComponentRegister::RegistComponents();
}

void GameInitializer::AddKey()
{
	// Qボタンで次元変更
	REngine::Input::Custom::AddButton(
		"ChangeDimention", REngine::Input::Key::Code::Q
	);
	REngine::Input::Custom::AddButton(
		"ChangeDimention", REngine::Input::Pad::Button::RShoulder
	);
}
