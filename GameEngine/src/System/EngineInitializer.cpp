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
#include "Common/TypeIdGenerator.h"
#include "FactoryRegister.h"

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

// 入力関連
#include "Input/InputSystem.h"
#include "Input/KeyInput.h"

//====================================================//
// 関数定義
//====================================================//

namespace REngine
{
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

		RegistCustomKeys();

		FactoryRegister::RegistFactories();
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

	void EngineInitializer::RegistCustomKeys()
	{
		//============== カスタム入力 ======================//

		Input::Custom::AddAxis(
			"Horizontal", Input::Key::Code::Right, Input::Key::Code::Left
		);
		Input::Custom::AddAxis(
			"Horizontal", Input::Key::Code::D, Input::Key::Code::A
		);
		Input::Custom::AddAxis(
			"Horizontal", Input::Pad::Axis::LeftStickX
		);

		Input::Custom::AddAxis(
			"Vertical", Input::Key::Code::Up, Input::Key::Code::Down
		);
		Input::Custom::AddAxis(
			"Vertical", Input::Key::Code::W, Input::Key::Code::S
		);
		Input::Custom::AddAxis(
			"Vertical", Input::Pad::Axis::LeftStickY
		);

		Input::Custom::AddButton(
			"Jump", Input::Key::Code::Space
		);
		Input::Custom::AddButton(
			"Jump", Input::Pad::Button::A
		);
	}
} // namespace REngine
