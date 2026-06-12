#include "pch.h"
#include "ComponentFactory.h"

#include <Game/GamePlayScene/Camera/TPSCamera.h>
#include <Game/GamePlayScene/Camera/ProjectionSmoothCamera.h>
#include <Game/GamePlayScene/ChangeDimention/ChangeColliderComponent.h>
#include <Game/GamePlayScene/Player/Player.h>

BaseComponent* ComponentFactory::CreateComponent(GameObject* owner, ComponentID id)
{
	switch (id)
	{
		// Transformは自動作成されるため特殊処理
	case ComponentID::Transform:
		return owner->GetComponent<Transform>();
	case ComponentID::BoxCollider:
		return owner->AddComponent<BoxCollider>();
	case ComponentID::SphereCollider:
		return owner->AddComponent<SphereCollider>();
	case ComponentID::CapsuleCollider:
		return owner->AddComponent<CapsuleCollider>();
	case ComponentID::BoxCollider2D:
		return owner->AddComponent<BoxCollider2D>();
	case ComponentID::CircleCollider2D:
		return owner->AddComponent<CircleCollider2D>();
	case ComponentID::CapsuleCollider2D:
		return owner->AddComponent<CapsuleCollider2D>();
	case ComponentID::ConvexPolygonCollider2D:
		return owner->AddComponent<ConvexPolygonCollider2D>();
	case ComponentID::RigidBody:
		return owner->AddComponent<RigidBody>();
	case ComponentID::RigidBody2D:
		return owner->AddComponent<RigidBody2D>();
	case ComponentID::ModelRenderer:
		return owner->AddComponent<ModelComponent>();
	case ComponentID::SpriteRenderer:
		return nullptr;	// Todo: 未実装 実装したら追加すること
	case ComponentID::StandardCamera:
		return owner->AddComponent<StandardCamera>();
	case ComponentID::TargetCamera:
		return owner->AddComponent<TargetCamera>();
	case ComponentID::ChangeColliderComponent:
		return owner->AddComponent<ChangeColliderComponent>();
	case ComponentID::Player:
		return owner->AddComponent<Player>();
	case ComponentID::TPSCamera:
		return owner->AddComponent<TPSCamera>();
	case ComponentID::ProjectionSmoothCamera:
		return owner->AddComponent<ProjectionSmoothCamera>();
	default:
		return nullptr;
	}
}
