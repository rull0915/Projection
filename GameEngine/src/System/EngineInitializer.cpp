//====================================================//
// ファイル名  : EngineInitializer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要        :  ゲームエンジン部分の初期化を担当するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"

#include "System/EngineInitializer.h"
#include "System/TypeIdGenerator.h"

// 衝突システム
#include "Managers/CollisionMap.h"
#include "Managers/3DManagers/Collider/CheckHit/StandardCheckHit.h"
#include "Managers/2DManagers/Collider/CheckHit/StandardCheckHit2D.h"

// コライダー
#include "Components/World/Collider/3D/Shapes/Colliders.h"
#include "Components/World/Collider/2D/Shapes/2DColliders.h"

// Ray関連
#include "Managers/3DManagers/Ray/RaySystem.h"
#include "Managers/3DManagers/Ray/CheckHitWithRay.h"

// 描画関連
#include "Debug/ColliderDebugRenderer.h"
#include "Debug/EachColliderDebugRenderer.h"

// 入力関連
#include "Input/InputSystem.h"
#include "Input/KeyInput.h"
#include "Input/MouseInput.h"

#include "System/ResourceManager.h"

//====================================================//
// 関数定義
//====================================================//

// staticメンバ変数定義
bool EngineInitializer::m_isInitialized = false;

// エンジン全体の初期化処理
void EngineInitializer::EngineInitialize()
{
	// 初期化済みなら何もしない
	if (m_isInitialized) return;

	// フラグをオンに
	m_isInitialized = true;

	Regist3DCollisions();

	Regist2DCollisions();

	RegistRayCollisions();

	RegistDebugDrawers();

	RegistCustomKeys();

	AddResources();
}

void EngineInitializer::Regist3DCollisions()
{
	// 球vs球
	CollisionMap::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		TypeIDGenerator::GetID<SphereCollider>(),
		Collision::CheckHit_Sphere_Sphere
	);

	// 球vsカプセル
	CollisionMap::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		TypeIDGenerator::GetID<CapsuleCollider>(),
		Collision::CheckHit_Sphere_Capsule
	);

	// 球vsボックス
	CollisionMap::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		TypeIDGenerator::GetID<BoxCollider>(),
		Collision::CheckHit_Sphere_Box
	);

	// カプセルvsカプセル
	CollisionMap::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		TypeIDGenerator::GetID<CapsuleCollider>(),
		Collision::CheckHit_Capsule_Capsule
	);

	// カプセルvsボックス
	CollisionMap::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		TypeIDGenerator::GetID<BoxCollider>(),
		Collision::CheckHit_Capsule_Box
	);

	// ボックスvsボックス
	CollisionMap::Register(
		TypeIDGenerator::GetID<BoxCollider>(),
		TypeIDGenerator::GetID<BoxCollider>(),
		Collision::CheckHit_Box_Box
	);
}

void EngineInitializer::Regist2DCollisions()
{
	// 円vs円
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		TypeIDGenerator::GetID<CircleCollider2D>(),
		Collision2D::CheckHit_Circle_Circle
	);

	// 円vsカプセル
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		Collision2D::CheckHit_Circle_Capsule
	);

	// 円vsボックス
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		TypeIDGenerator::GetID<BoxCollider2D>(),
		Collision2D::CheckHit_Circle_Box
	);

	// カプセルvsカプセル
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		Collision2D::CheckHit_Capsule_Capsule
	);

	// カプセルvsボックス
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		TypeIDGenerator::GetID<BoxCollider2D>(),
		Collision2D::CheckHit_Capsule_Box
	);

	// ボックスvsボックス
	CollisionMap2D::Register(
		TypeIDGenerator::GetID<BoxCollider2D>(),
		TypeIDGenerator::GetID<BoxCollider2D>(),
		Collision2D::CheckHit_Box_Box
	);
}

void EngineInitializer::RegistRayCollisions()
{
	// Ray vs Sphere
	RaySystem::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		RayCollision::CheckHit_Ray_Sphere
	);

	// Ray vs Capsule
	RaySystem::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		RayCollision::CheckHit_Ray_Capsule
	);

	// Ray vs Box
	RaySystem::Register(
		TypeIDGenerator::GetID<BoxCollider>(),
		RayCollision::CheckHit_Ray_Box
	);
}

void EngineInitializer::RegistDebugDrawers()
{
	//============== DebugDraw =================//

	// Sphere
	ColliderDebugRenderer::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		DrawCollider::DrawCollider_Sphere
	);

	// Capsule
	ColliderDebugRenderer::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		DrawCollider::DrawCollider_Capsule
	);

	// Box
	ColliderDebugRenderer::Register(
		TypeIDGenerator::GetID<BoxCollider>(),
		DrawCollider::DrawCollider_Box
	);

	//============== DebugDraw2D =================//

	// Circle
	ColliderDebugRenderer2D::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		DrawCollider2D::DrawCollider_Circle
	);

	// Capsule
	ColliderDebugRenderer2D::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		DrawCollider2D::DrawCollider_Capsule
	);

	// Box
	ColliderDebugRenderer2D::Register(
		TypeIDGenerator::GetID<BoxCollider2D>(),
		DrawCollider2D::DrawCollider_Box
	);
}

void EngineInitializer::RegistCustomKeys()
{
	//============== カスタム入力 ======================//

	Input::Custom::AddAxis(
		"Horizontal", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::Left), Input::Axis::Side::Negative
	);
	Input::Custom::AddAxis(
		"Horizontal", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::Right), Input::Axis::Side::Positive
	);
	Input::Custom::AddAxis(
		"Horizontal", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::A), Input::Axis::Side::Negative
	);
	Input::Custom::AddAxis(
		"Horizontal", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::D), Input::Axis::Side::Positive
	);

	Input::Custom::AddAxis(
		"Vertical", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::Down), Input::Axis::Side::Negative
	);
	Input::Custom::AddAxis(
		"Vertical", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::Up), Input::Axis::Side::Positive
	);
	Input::Custom::AddAxis(
		"Vertical", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::S), Input::Axis::Side::Negative
	);
	Input::Custom::AddAxis(
		"Vertical", Input::Bind(Input::Type::Key, Input::State::Press, Input::Key::Code::W), Input::Axis::Side::Positive
	);

	Input::Custom::AddButton(
		"Jump", Input::Bind(Input::Type::Key, Input::State::Down, Input::Key::Code::Space)
	);
}

void EngineInitializer::AddResources()
{
	// テンプレートオブジェクト用モデル
	ResourceManager::Instance().AddModel("Cube", L"Resources/Models/Cube.cmo");
	ResourceManager::Instance().AddModel("Sphere", L"Resources/Models/Sphere.cmo");
	ResourceManager::Instance().AddModel("Capsule", L"Resources/Models/Capsule.cmo");

	// デフォルトフォント
	ResourceManager::Instance().AddFont("Default", L"Resources/Fonts/Default.spritefont");

	// デフォルトイメージ
	ResourceManager::Instance().AddTexture("Default", L"Resources/Textures/Default.png");
}
