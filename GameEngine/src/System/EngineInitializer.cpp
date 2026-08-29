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
			SphereCollider::StaticTypeId(),
			SphereCollider::StaticTypeId(),
			Collision::CheckHit_Sphere_Sphere
		);

		// 球vsカプセル
		CollisionMap::Register(
			SphereCollider::StaticTypeId(),
			CapsuleCollider::StaticTypeId(),
			Collision::CheckHit_Sphere_Capsule
		);

		// 球vsボックス
		CollisionMap::Register(
			SphereCollider::StaticTypeId(),
			BoxCollider::StaticTypeId(),
			Collision::CheckHit_Sphere_Box
		);

		// カプセルvsカプセル
		CollisionMap::Register(
			CapsuleCollider::StaticTypeId(),
			CapsuleCollider::StaticTypeId(),
			Collision::CheckHit_Capsule_Capsule
		);

		// カプセルvsボックス
		CollisionMap::Register(
			CapsuleCollider::StaticTypeId(),
			BoxCollider::StaticTypeId(),
			Collision::CheckHit_Capsule_Box
		);

		// ボックスvsボックス
		CollisionMap::Register(
			BoxCollider::StaticTypeId(),
			BoxCollider::StaticTypeId(),
			Collision::CheckHit_Box_Box
		);
	}

	void EngineInitializer::Regist2DCollisions()
	{
		// 円vs円
		CollisionMap2D::Register(
			CircleCollider2D::StaticTypeId(),
			CircleCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Circle_Circle
		);

		// 円vsカプセル
		CollisionMap2D::Register(
			CircleCollider2D::StaticTypeId(),
			CapsuleCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Circle_Capsule
		);

		// 円vsボックス
		CollisionMap2D::Register(
			CircleCollider2D::StaticTypeId(),
			BoxCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Circle_Box
		);

		// カプセルvsカプセル
		CollisionMap2D::Register(
			CapsuleCollider2D::StaticTypeId(),
			CapsuleCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Capsule_Capsule
		);

		// カプセルvsボックス
		CollisionMap2D::Register(
			CapsuleCollider2D::StaticTypeId(),
			BoxCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Capsule_Box
		);

		// ボックスvsボックス
		CollisionMap2D::Register(
			BoxCollider2D::StaticTypeId(),
			BoxCollider2D::StaticTypeId(),
			Collision2D::CheckHit_Box_Box
		);
	}

	void EngineInitializer::RegistRayCollisions()
	{
		// Ray vs Sphere
		RaySystem::Register(
			SphereCollider::StaticTypeId(),
			RayCollision::CheckHit_Ray_Sphere
		);

		// Ray vs Capsule
		RaySystem::Register(
			CapsuleCollider::StaticTypeId(),
			RayCollision::CheckHit_Ray_Capsule
		);

		// Ray vs Box
		RaySystem::Register(
			BoxCollider::StaticTypeId(),
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

		Input::Custom::AddAxis(
			"CameraX", Input::Pad::Axis::RightStickX, 2.0f
		);
		Input::Custom::AddAxis(
			"CameraX", Input::Mouse::Axis::X
		);

		Input::Custom::AddAxis(
			"CameraY", Input::Pad::Axis::RightStickY, -2.0f
		);
		Input::Custom::AddAxis(
			"CameraY", Input::Mouse::Axis::Y
		);
	}
} // namespace REngine
