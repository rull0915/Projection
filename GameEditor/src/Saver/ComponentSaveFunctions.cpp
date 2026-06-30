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
#include "ComponentSaveFunctions.h"

#include "Components/World/Components.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace SaveComponent
{
	using namespace DirectX;

	// 基底
	json Save_Base(ComponentBase* component)
	{
		json j;
		j["IsActive"] = component->IsActive();
		return j;
	}

	// Transform
	json Save_Transform(ComponentBase* component)
	{
		// トランスフォームに変換
		Transform* transform = static_cast<Transform*>(component);

		// 情報の取得
		SimpleMath::Vector3 pos = transform->GetLocalPosition();
		SimpleMath::Quaternion rot = transform->GetLocalRotation();
		SimpleMath::Vector3 sca = transform->GetLocalScale();

		// jsonを用意
		json j = Save_Base(component);

		j["Position"] = { pos.x, pos.y, pos.z };
		j["Rotation"] = { rot.x, rot.y, rot.z, rot.w };
		j["Scale"] = { sca.x, sca.y, sca.z };

		return j;
	}

	json Save_Collider_Common(ComponentBase* component)
	{
		json j = Save_Base(component);

		// 変換
		ColliderCommon* collider = static_cast<ColliderCommon*>(component);

		j["Layer"] = collider->GetLayer();
		j["NeedInfo"] = collider->IsNeedInfo();
		j["Trigger"] = collider->IsTrigger();

		return j;
	}

	// ColliderBase
	json Save_Collider_Base(ComponentBase* component)
	{
		json j = Save_Collider_Common(component);

		// 変換
		ColliderBase* collider = static_cast<ColliderBase*>(component);

		DirectX::SimpleMath::Vector3 local = collider->GetLocalCenterPos();

		// 保存
		j["LocalPosition"] = { local.x, local.y, local.z };

		return j;
	}

	// BoxCollider
	json Save_Box_Collider(ComponentBase* component)
	{
		json j = Save_Collider_Base(component);

		// 変換
		BoxCollider* collider = static_cast<BoxCollider*>(component);

		// 保存
		DirectX::SimpleMath::Vector3 localSize = collider->GetLocalSize();

		j["LocalSize"] = { localSize.x, localSize.y, localSize.z };

		return j;
	}

	// CapsuleCollider
	json Save_Capsule_Collider(ComponentBase* component)
	{
		json j = Save_Collider_Base(component);

		// 変換
		CapsuleCollider* collider = static_cast<CapsuleCollider*>(component);

		// 保存
		j["Height"] = collider->GetLocalHeight();
		j["Radius"] = collider->GetLocalRadius();
		j["LineDir"] = static_cast<int>(collider->GetLineAxis());

		return j;
	}

	// SphereCollider
	json Save_Sphere_Collider(ComponentBase* component)
	{
		json j = Save_Collider_Base(component);

		// 変換
		SphereCollider* collider = static_cast<SphereCollider*>(component);

		// 保存
		j["Radius"] = collider->GetLocalRadius();

		return j;
	}

	// ColliderBase2D
	json Save_Collider_Base_2D(ComponentBase* component)
	{
		json j = Save_Collider_Common(component);

		// 変換
		ColliderBase2D* collider = static_cast<ColliderBase2D*>(component);

		DirectX::SimpleMath::Vector2 local = collider->GetLocalCenterPos();

		// 保存
		j["LocalPosition"] = { local.x, local.y };

		return j;
	}

	// BoxCollider2D
	json Save_Box_Collider_2D(ComponentBase* component)
	{
		json j = Save_Collider_Base_2D(component);

		// 変換
		BoxCollider2D* collider = static_cast<BoxCollider2D*>(component);

		// 保存
		DirectX::SimpleMath::Vector2 localSize = collider->GetLocalSize();

		j["LocalSize"] = { localSize.x, localSize.y };

		return j;
	}

	// CapsuleCollider2D
	json Save_Capsule_Collider_2D(ComponentBase* component)
	{
		json j = Save_Collider_Base_2D(component);

		// 変換
		CapsuleCollider2D* collider = static_cast<CapsuleCollider2D*>(component);

		// 保存
		j["Height"] = collider->GetHeight();
		j["Radius"] = collider->GetRadius();
		j["LineDir"] = static_cast<int>(collider->GetAxis());

		return j;
	}

	// CircleCollider2D
	json Save_Circle_Collider_2D(ComponentBase* component)
	{
		json j = Save_Collider_Base_2D(component);

		// 変換
		CircleCollider2D* collider = static_cast<CircleCollider2D*>(component);

		// 保存
		j["Radius"] = collider->GetRadius();

		return j;
	}

	// RigidBodyBase
	json Save_RigidBody_Base(ComponentBase* component)
	{
		json j = Save_Base(component);

		// 変換
		RigidBodyBase* collider = static_cast<RigidBodyBase*>(component);

		j["UseGravity"] = collider->IsUseGravity();
		j["Static"] = collider->IsStatic();
		j["Mass"] = collider->GetMass();
		j["LinearDamping"] = collider->GetLinearDamping();

		return j;
	}

	// RigidBody
	json Save_RigidBody(ComponentBase* component)
	{
		return Save_RigidBody_Base(component);
	}

	// RigidBody
	json Save_RigidBody_2D(ComponentBase* component)
	{
		return Save_RigidBody_Base(component);
	}

	// Renderer
	json Save_Renderer(ComponentBase* component)
	{
		json j = Save_Base(component);

		// 変換
		RendererBase* renderer = static_cast<RendererBase*>(component);

		j["Alpha"] = renderer->GetAlpha();

		return j;
	}

	// ModelRenderer
	json Save_Model_Renderer(ComponentBase* component)
	{
		json j = Save_Renderer(component);

		// 変換
		ModelComponent* renderer = static_cast<ModelComponent*>(component);

		j["ModelName"] = renderer->GetModelName();

		return j;
	}

	// CameraBase
	json Save_Camera_Base(ComponentBase* component)
	{
		json j = Save_Base(component);

		// 変換
		CameraBase* camera = static_cast<CameraBase*>(component);

		return j;
	}

	// StandardCamera
	json Save_Standard_Camera(ComponentBase* component)
	{
		json j = Save_Camera_Base(component);

		// 変換
		StandardCamera* camera = static_cast<StandardCamera*>(component);

		// 保存
		j["Fov"] = camera->GetFov();
		j["Far"] = camera->GetFarClip();
		j["Near"] = camera->GetNearClip();
		j["Aspect"] = camera->GetAspect();

		return j;
	}

	// TargetCamera
	json Save_Target_Camera(ComponentBase* component)
	{
		json j = Save_Camera_Base(component);

		// 変換
		TargetCamera* camera = static_cast<TargetCamera*>(component);

		// 保存
		j["Fov"] = camera->GetFov();
		j["Far"] = camera->GetFarClip();
		j["Near"] = camera->GetNearClip();
		j["Aspect"] = camera->GetAspect();

		return j;
	}
}
