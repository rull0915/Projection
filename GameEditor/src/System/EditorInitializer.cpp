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
#include "EditorInitializer.h"

#include "Saver/ComponentSaveFunctions.h"
#include "Saver/ObjectSaver.h"
#include "System/TypeIdGenerator.h"

#include "Components/World/Components.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EditorInitializer::Initialize()
{
	// 各コンポーネントの保存関数の登録

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
