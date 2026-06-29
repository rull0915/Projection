//====================================================//
// ファイル名   : ComponentSaveFunctionsScene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : 各コンポーネントの保存関数をまとめたヘッダ
//
// 更新履歴 : 
// 2026/06/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <nlohmann/json.hpp>
#include "Components/ComponentBase.h"

// 保存関数名前空間
namespace SaveComponent
{
	using json = nlohmann::json;

	// 基底
	json Save_Base(ComponentBase* component);

	// Transform
	json Save_Transform(ComponentBase* component);

	// ColliderCommon
	json Save_Collider_Common(ComponentBase* component);

	// ColliderBase
	json Save_Collider_Base(ComponentBase* component);

	// BoxCollider
	json Save_Box_Collider(ComponentBase* component);

	// CapsuleCollider
	json Save_Capsule_Collider(ComponentBase* component);

	// SphereCollider
	json Save_Sphere_Collider(ComponentBase* component);

	// ColliderBase2D
	json Save_Collider_Base_2D(ComponentBase* component);

	// BoxCollider2D
	json Save_Box_Collider_2D(ComponentBase* component);

	// CapsuleCollider2D
	json Save_Capsule_Collider_2D(ComponentBase* component);

	// CircleCollider2D
	json Save_Circle_Collider_2D(ComponentBase* component);

	// RigidBodyBase
	json Save_RigidBody_Base(ComponentBase* component);

	// RigidBody
	json Save_RigidBody(ComponentBase* component);

	// RigidBody
	json Save_RigidBody_2D(ComponentBase* component);

	// Renderer
	json Save_Renderer(ComponentBase* component);

	// ModelRenderer
	json Save_Model_Renderer(ComponentBase* component);

	// CameraBase
	json Save_Camera_Base(ComponentBase* component);

	// StandardCamere
	json Save_Standard_Camera(ComponentBase* component);

	// TargetCamere
	json Save_Target_Camera(ComponentBase* component);
}
