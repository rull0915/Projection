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
#include "Input/InputSystem.h"
#include "Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void GameInitializer::Initialize()
{
	// 衝突判定関数の追加
	RHEngine::CollisionMap2D::Register(
		ConvexPolygonCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
		Collision2D::CheckHit2D_Convex_Convex
	);
	RHEngine::CollisionMap2D::Register(
		RHEngine::CircleCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
		Collision2D::CheckHit2D_Circle_Convex
	);
	RHEngine::CollisionMap2D::Register(
		RHEngine::CapsuleCollider2D::StaticTypeId(), ConvexPolygonCollider2D::StaticTypeId(),
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
	RHEngine::Input::Custom::AddButton(
		"ChangeDimention", RHEngine::Input::Key::Code::Q
	);
	RHEngine::Input::Custom::AddButton(
		"ChangeDimention", RHEngine::Input::Pad::Button::RShoulder
	);

	// 決定
	RHEngine::Input::Custom::AddButton(
		"Decide", RHEngine::Input::Key::Code::Space
	);
	RHEngine::Input::Custom::AddButton(
		"Decide", RHEngine::Input::Key::Code::Z
	);
	RHEngine::Input::Custom::AddButton(
		"Decide", RHEngine::Input::Pad::Button::B
	);
}
