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
	void Load_Base(const json& js, ComponentBase* component);

	// Transform
	void Load_Transform(const json& js, ComponentBase* component);

	// ColliderCommon
	void Load_Collider_Common(const json& js, ComponentBase* component);

	// ColliderBase
	void Load_Collider_Base(const json& js, ComponentBase* component);

	// BoxCollider
	void Load_Box_Collider(const json& js, ComponentBase* component);

	// CapsuleCollider
	void Load_Capsule_Collider(const json& js, ComponentBase* component);

	// SphereCollider
	void Load_Sphere_Collider(const json& js, ComponentBase* component);

	// ColliderBase2D
	void Load_Collider_Base_2D(const json& js, ComponentBase* component);

	// BoxCollider2D
	void Load_Box_Collider_2D(const json& js, ComponentBase* component);

	// CapsuleCollider2D
	void Load_Capsule_Collider_2D(const json& js, ComponentBase* component);

	// CircleCollider2D
	void Load_Circle_Collider_2D(const json& js, ComponentBase* component);

	// RigidBodyBase
	void Load_RigidBody_Base(const json& js, ComponentBase* component);

	// RigidBody
	void Load_RigidBody(const json& js, ComponentBase* component);

	// RigidBody
	void Load_RigidBody_2D(const json& js, ComponentBase* component);

	// Renderer
	void Load_Renderer(const json& js, ComponentBase* component);

	// ModelRenderer
	void Load_Model_Renderer(const json& js, ComponentBase* component);

	// CameraBase
	void Load_Camera_Base(const json& js, ComponentBase* component);

	// StandardCamere
	void Load_Standard_Camera(const json& js, ComponentBase* component);

	// TargetCamere
	void Load_Target_Camera(const json& js, ComponentBase* component);
}
