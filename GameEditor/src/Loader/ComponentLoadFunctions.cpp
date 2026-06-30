//====================================================//
// ファイル名  : ComponentSaveFunctions.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要       : 各コンポーネントの保存関数をまとめたヘッダ
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ComponentLoadFunctions.h"

#include "Components/World/Components.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace SaveComponent
{
	using namespace DirectX;
	using namespace nlohmann;

	// 基底
	void Load_Base(const json& json, ComponentBase* component)
	{
		component->SetActive(json["IsActive"]);
	}

	// Transform
	void Load_Transform(const json& json, ComponentBase* component)
	{
		Load_Base(json, component);

		// トランスフォームに変換
		Transform* transform = static_cast<Transform*>(component);

		// 情報のセット
		transform->SetLocalPosition({ json["Position"][0], json["Position"][1], json["Position"][2] });
		transform->SetLocalRotation({ json["Rotation"][0], json["Rotation"][1], json["Rotation"][2], json["Rotation"][3] });
		transform->SetLocalScale({ json["Scale"][0], json["Scale"][1], json["Scale"][2] });
	}

	void Load_Collider_Common(const json& json, ComponentBase* component)
	{
		Load_Base(json, component);

		// 変換
		ColliderCommon* collider = static_cast<ColliderCommon*>(component);

		collider->SetLayer(json["Layer"]);
		collider->SetNeedInfo(json["NeedInfo"]);
		collider->SetTrigger(json["Trigger"]);
	}

	// ColliderBase
	void Load_Collider_Base(const json& json, ComponentBase* component)
	{
		Load_Collider_Common(json, component);

		// 変換
		ColliderBase* collider = static_cast<ColliderBase*>(component);

		collider->SetLocalPos({ json["LocalPosition"][0], json["LocalPosition"][1], json["LocalPosition"][2] });
	}

	// BoxCollider
	void Load_Box_Collider(const json& json, ComponentBase* component)
	{
		Load_Collider_Base(json, component);

		// 変換
		BoxCollider* collider = static_cast<BoxCollider*>(component);

		collider->SetLocalSize({ json["LocalSize"][0], json["LocalSize"][1], json["LocalSize"][2] });
	}

	// CapsuleCollider
	void Load_Capsule_Collider(const json& json, ComponentBase* component)
	{
		Load_Collider_Base(json, component);

		// 変換
		CapsuleCollider* collider = static_cast<CapsuleCollider*>(component);

		collider->SetHeight(json["Height"]);
		collider->SetRadius(json["Radius"]);
		collider->SetLineAxis(static_cast<AxisType>(json["LineDir"]));
	}

	// SphereCollider
	void Load_Sphere_Collider(const json& json, ComponentBase* component)
	{
		Load_Collider_Base(json, component);

		// 変換
		SphereCollider* collider = static_cast<SphereCollider*>(component);

		collider->SetRadius(json["Radius"]);
	}

	// ColliderBase2D
	void Load_Collider_Base_2D(const json& json, ComponentBase* component)
	{
		Load_Collider_Common(json, component);

		// 変換
		ColliderBase2D* collider = static_cast<ColliderBase2D*>(component);

		collider->SetLocalPos({ json["LocalPosition"][0], json["LocalPosition"][1] });
	}

	// BoxCollider2D
	void Load_Box_Collider_2D(const json& json, ComponentBase* component)
	{
		Load_Collider_Base_2D(json, component);

		// 変換
		BoxCollider2D* collider = static_cast<BoxCollider2D*>(component);

		collider->SetLocalSize({ json["LocalSize"][0], json["LocalSize"][1] });
	}

	// CapsuleCollider2D
	void Load_Capsule_Collider_2D(const json& json, ComponentBase* component)
	{
		Load_Collider_Base_2D(json, component);

		// 変換
		CapsuleCollider2D* collider = static_cast<CapsuleCollider2D*>(component);

		collider->SetHeight(json["Height"]);
		collider->SetRadius(json["Radius"]);
		collider->SetLineDir(static_cast<AxisType2D>(json["LineDir"]));
	}

	// CircleCollider2D
	void Load_Circle_Collider_2D(const json& json, ComponentBase* component)
	{
		Load_Collider_Base_2D(json, component);

		// 変換
		CircleCollider2D* collider = static_cast<CircleCollider2D*>(component);

		collider->SetRadius(json["Radius"]);
	}

	// RigidBodyBase
	void Load_RigidBody_Base(const json& json, ComponentBase* component)
	{
		Load_Base(json, component);

		// 変換
		RigidBodyBase* rigidbody = static_cast<RigidBodyBase*>(component);

		rigidbody->SetUseGravity(json["UseGravity"]);
		rigidbody->SetStatic(json["Static"]);
		rigidbody->SetMass(json["Mass"]);
		rigidbody->SetLinearDamping(json["LinearDamping"]);
	}

	// RigidBody
	void Load_RigidBody(const json& json, ComponentBase* component)
	{
		Load_RigidBody_Base(json, component);
	}

	// RigidBody
	void Load_RigidBody_2D(const json& json, ComponentBase* component)
	{
		Load_RigidBody_Base(json, component);
	}

	// Renderer
	void Load_Renderer(const json& json, ComponentBase* component)
	{
		Load_Base(json, component);

		// 変換
		RendererBase* renderer = static_cast<RendererBase*>(component);

		renderer->SetAlpha(json["Alpha"]);
	}

	// ModelRenderer
	void Load_Model_Renderer(const json& json, ComponentBase* component)
	{
		Load_Renderer(json, component);

		// 変換
		ModelComponent* renderer = static_cast<ModelComponent*>(component);

		renderer->SetModel(json["ModelName"]);
	}

	// CameraBase
	void Load_Camera_Base(const json& json, ComponentBase* component)
	{
		Load_Base(json, component);
	}

	// StandardCamera
	void Load_Standard_Camera(const json& json, ComponentBase* component)
	{
		Load_Camera_Base(json, component);

		// 変換
		StandardCamera* camera = static_cast<StandardCamera*>(component);

		// 保存
		camera->SetFov(json["Fov"]);
		camera->SetFarClip(json["Far"]);
		camera->SetNearClip(json["Near"]);
		camera->SetAspect(json["Aspect"]);
	}

	// TargetCamera
	void Load_Target_Camera(const json& json, ComponentBase* component)
	{
		Load_Camera_Base(json, component);

		// 変換
		TargetCamera* camera = static_cast<TargetCamera*>(component);

		// 保存
		camera->SetFov(json["Fov"]);
		camera->SetFarClip(json["Far"]);
		camera->SetNearClip(json["Near"]);
		camera->SetAspect(json["Aspect"]);
	}
}
