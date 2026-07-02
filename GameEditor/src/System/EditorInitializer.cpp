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

// 生成
#include "Loader/ComponentFactory.h"

#include "Components/World/Components.h"
#include "GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EditorInitializer::Initialize()
{
	// 各コンポーネントの生成関数の登録
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

	// AudioSource
	ComponentFactory::Register(
		"AudioSource", [](GameObject* o) { return o->AddComponent<AudioSource>(); }
	);
}
