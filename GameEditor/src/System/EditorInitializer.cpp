//====================================================//
// ファイル名  : EditorInitializer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/EditorInitializer.h"
#include "System/TypeIdGenerator.h"

// 保存
#include "Saver/ComponentSaveFunctions.h"
#include "Saver/ObjectSaver.h"

// ロード
#include "Loader/ComponentLoadFunctions.h"
#include "Loader/ObjectLoader.h"

// 生成
#include "Loader/ComponentFactory.h"

#include "Components/World/Components.h"

#include "GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EditorInitializer::Initialize()
{
	// 各コンポーネントの保存関数の登録
	RegistLoaders();

	RegistSavers();

	RegistFactories();
}

void EditorInitializer::RegistFactories()
{
	// Transform
	ComponentFactory::Register(
		"Transform", [](GameObject* o) { return o->AddComponent<Transform>(); }
	);

	// Collider
	ComponentFactory::Register(
		"BoxCollider", [](GameObject* o) { return o->AddComponent<BoxCollider>(); }
	);
	ComponentFactory::Register(
		"CapsuleCollider", [](GameObject* o) { return o->AddComponent<CapsuleCollider>(); }
	);
	ComponentFactory::Register(
		"SphereCollider", [](GameObject* o) { return o->AddComponent<SphereCollider>(); }
	);

	// Collider2D
	ComponentFactory::Register(
		"BoxCollider2D", [](GameObject* o) { return o->AddComponent<BoxCollider2D>(); }
	);
	ComponentFactory::Register(
		"CapsuleCollider2D", [](GameObject* o) { return o->AddComponent<CapsuleCollider2D>(); }
	);
	ComponentFactory::Register(
		"CircleCollider2D", [](GameObject* o) { return o->AddComponent<CircleCollider2D>(); }
	);

	// RigidBody
	ComponentFactory::Register(
		"RigidBody", [](GameObject* o) { return o->AddComponent<RigidBody>(); }
	);
	ComponentFactory::Register(
		"RigidBody2D", [](GameObject* o) { return o->AddComponent<RigidBody2D>(); }
	);

	// Renderer
	ComponentFactory::Register(
		"ModelComponent", [](GameObject* o) { return o->AddComponent<ModelComponent>(); }
	);

	// Camera
	ComponentFactory::Register(
		"StandardCamera", [](GameObject* o) { return o->AddComponent<StandardCamera>(); }
	);
	ComponentFactory::Register(
		"TargetCamera", [](GameObject* o) { return o->AddComponent<TargetCamera>(); }
	);
}

void EditorInitializer::RegistSavers()
{
	// Transform
	ObjectSaver::Register(
		TypeIDGenerator::GetID<Transform>(),
		SaveComponent::Save_Transform
	);

	// Collider
	ObjectSaver::Register(
		TypeIDGenerator::GetID<BoxCollider>(),
		SaveComponent::Save_Box_Collider
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		SaveComponent::Save_Capsule_Collider
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		SaveComponent::Save_Sphere_Collider
	);

	// Collider2D
	ObjectSaver::Register(
		TypeIDGenerator::GetID<BoxCollider2D>(),
		SaveComponent::Save_Box_Collider_2D
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		SaveComponent::Save_Capsule_Collider_2D
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		SaveComponent::Save_Circle_Collider_2D
	);

	// RigidBody
	ObjectSaver::Register(
		TypeIDGenerator::GetID<RigidBody>(),
		SaveComponent::Save_RigidBody
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<RigidBody2D>(),
		SaveComponent::Save_RigidBody_2D
	);

	// Renderer
	ObjectSaver::Register(
		TypeIDGenerator::GetID<ModelComponent>(),
		SaveComponent::Save_Model_Renderer
	);

	// Camera
	ObjectSaver::Register(
		TypeIDGenerator::GetID<StandardCamera>(),
		SaveComponent::Save_Standard_Camera
	);
	ObjectSaver::Register(
		TypeIDGenerator::GetID<TargetCamera>(),
		SaveComponent::Save_Target_Camera
	);
}

void EditorInitializer::RegistLoaders()
{
	// Transform
	ObjectLoader::Register(
		TypeIDGenerator::GetID<Transform>(),
		SaveComponent::Load_Transform
	);

	// Collider
	ObjectLoader::Register(
		TypeIDGenerator::GetID<BoxCollider>(),
		SaveComponent::Load_Box_Collider
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<CapsuleCollider>(),
		SaveComponent::Load_Capsule_Collider
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<SphereCollider>(),
		SaveComponent::Load_Sphere_Collider
	);

	// Collider2D
	ObjectLoader::Register(
		TypeIDGenerator::GetID<BoxCollider2D>(),
		SaveComponent::Load_Box_Collider_2D
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<CapsuleCollider2D>(),
		SaveComponent::Load_Capsule_Collider_2D
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<CircleCollider2D>(),
		SaveComponent::Load_Circle_Collider_2D
	);

	// RigidBody
	ObjectLoader::Register(
		TypeIDGenerator::GetID<RigidBody>(),
		SaveComponent::Load_RigidBody
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<RigidBody2D>(),
		SaveComponent::Load_RigidBody_2D
	);

	// Renderer
	ObjectLoader::Register(
		TypeIDGenerator::GetID<ModelComponent>(),
		SaveComponent::Load_Model_Renderer
	);

	// Camera
	ObjectLoader::Register(
		TypeIDGenerator::GetID<StandardCamera>(),
		SaveComponent::Load_Standard_Camera
	);
	ObjectLoader::Register(
		TypeIDGenerator::GetID<TargetCamera>(),
		SaveComponent::Load_Target_Camera
	);
}
