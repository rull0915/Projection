//====================================================//
// ファイル名  : FactoryRegister.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要       : ファクトリへ関数を登録するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/FactoryRegister.h"

#include "Components/World/Components.h"
#include "Components/UI/UIComponents.h"
#include "Components/Both/BothComponents.h"

#include "Editor/Loader/ComponentFactory.h"
#include "GameObject/GameObject.h"
#include "Components/ComponentInfo.h"

#define ADD_FACTORY(type) ComponentFactory::Register(#type, { ComponentProject::Engine, type::SPACE }, [](GameObject* o) { return o->AddComponent<type>(); });

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void FactoryRegister::RegistFactories()
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

		// UI
		ADD_FACTORY(Canvas);
		ADD_FACTORY(RectTransform);
		ADD_FACTORY(ImageUI);
		ADD_FACTORY(TextUI);
		ADD_FACTORY(ButtonUI);
	}
}	// namespace REngine
