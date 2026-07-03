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

#define ADD_FACTORY(type) ComponentFactory::Register(#type, [](GameObject* o) { return o->AddComponent<type>(); });

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
	ADD_FACTORY(Transform);

	// Collider
	ADD_FACTORY(BoxCollider);
	ADD_FACTORY(CapsuleCollider);
	ADD_FACTORY(SphereCollider);

	// Collider2D
	ADD_FACTORY(BoxCollider2D);
	ADD_FACTORY(CapsuleCollider2D);
	ADD_FACTORY(CircleCollider2D);

	// RigidBody
	ADD_FACTORY(RigidBody);
	ADD_FACTORY(RigidBody2D);

	// Renderer
	ADD_FACTORY(ModelComponent);
	ADD_FACTORY(SkyboxComponent);

	// Camera
	ADD_FACTORY(StandardCamera);
	ADD_FACTORY(TargetCamera);

	// Audio
	ADD_FACTORY(AudioSource);
	ADD_FACTORY(AudioListener);
}
