//====================================================//
// ファイル名  : ComponentRegister.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ComponentRegister.h"

#include "Scene/Scene.h"
#include "Components/ComponentBase.h"

#include "System/TypeIdGenerator.h"

#include "Managers/CameraManager.h"
#include "Managers/Renderer/RendererManager.h"
#include "Managers/2DManagers/PhysicsManager2D.h"
#include "Managers/2DManagers/Collider/CollideManager2D.h"
#include "Managers/3DManagers/PhysicsManager.h"
#include "Managers/3DManagers/Collider/CollideManager.h"
#include "Managers/System/CollideEventSystem.h"
#include "Managers/Sounds/SoundManager.h"

//====================================================//
// 前方宣言
//====================================================//
class RigidBody;
class RigidBody2D;

//====================================================//
// 関数の実体宣言
//====================================================//

ComponentRegister::ComponentRegister(Scene* scene)
	: m_pScene{ scene }
{
}

void ComponentRegister::RegisterComponent(ComponentBase* component)
{
	// カテゴリで分類
	switch (component->GetCategory())
	{
		// カテゴリに分類されないコンポーネントの場合
	case Category::Original:

		// IDで識別

		// RigidBodyの場合
		if (component->GetID() == TypeIDGenerator::GetID<RigidBody>()) {
			m_pScene->m_physicsManager->AddRigidBody(static_cast<RigidBody*>(component));
			break;
		}

		// RigidBody2Dの場合
		if (component->GetID() == TypeIDGenerator::GetID<RigidBody2D>()) {
			m_pScene->m_physicsManager2D->AddRigidBody(static_cast<RigidBody2D*>(component));
			break;
		}

		// AudioSourceの場合
		if (component->GetID() == TypeIDGenerator::GetID<AudioSource>()) {
			m_pScene->m_soundManager->AddAudioSource(static_cast<AudioSource*>(component));
			break;
		}

		// AudioListenerの場合
		if (component->GetID() == TypeIDGenerator::GetID<AudioListener>()) {
			m_pScene->m_soundManager->SetListener(static_cast<AudioListener*>(component));
			break;
		}

		break;

		// カメラコンポーネントの場合
	case Category::Camera:
		m_pScene->m_cameraManager->SetMainCamera(static_cast<CameraBase*>(component));
		break;

		// 3Dコライダーの場合
	case Category::Collider:
		m_pScene->m_physicsManager->GetCollideManager()->AddCollide(static_cast<ColliderBase*>(component));
		break;

		// 2Dコライダーの場合
	case Category::Collider2D:
		m_pScene->m_physicsManager2D->GetCollideManager()->AddCollide(static_cast<ColliderBase2D*>(component));
		break;

		// 描画コンポーネントの場合
	case Category::Renderer:
		m_pScene->m_rendererManager->AddRenderer(static_cast<RendererBase*>(component));
		break;
	}
}

void ComponentRegister::UnRegisterComponent(ComponentBase* component)
{
	// カテゴリで分類
	switch (component->GetCategory())
	{
		// カテゴリに分類されないコンポーネントの場合
	case Category::Original:

		// IDで識別

		// RigidBodyの場合
		if (component->GetID() == TypeIDGenerator::GetID<RigidBody>()) {
			m_pScene->m_physicsManager->RemoveRigidBody(static_cast<RigidBody*>(component));
			break;
		}

		// RigidBody2Dの場合
		if (component->GetID() == TypeIDGenerator::GetID<RigidBody2D>()) {
			m_pScene->m_physicsManager2D->RemoveRigidBody(static_cast<RigidBody2D*>(component));
			break;
		}

		// AudioSourceの場合
		if (component->GetID() == TypeIDGenerator::GetID<AudioSource>()) {
			m_pScene->m_soundManager->RemoveAudioSource(static_cast<AudioSource*>(component));
			break;
		}

		// AudioListenerの場合
		if (component->GetID() == TypeIDGenerator::GetID<AudioListener>()) {
			m_pScene->m_soundManager->RemoveListener(static_cast<AudioListener*>(component));
			break;
		}

		break;

		// カメラコンポーネントの場合
	case Category::Camera:
		m_pScene->m_cameraManager->UnSetMainCamera(static_cast<CameraBase*>(component));
		break;

		// 3Dコライダーの場合
	case Category::Collider:
		m_pScene->m_physicsManager->GetCollideManager()->RemoveCollide(static_cast<ColliderBase*>(component));
		m_pScene->m_colEvent->RemoveCollider(static_cast<ColliderBase*>(component));
		break;

		// 2Dコライダーの場合
	case Category::Collider2D:
		m_pScene->m_physicsManager2D->GetCollideManager()->RemoveCollide(static_cast<ColliderBase2D*>(component));
		m_pScene->m_colEvent->RemoveCollider2D(static_cast<ColliderBase2D*>(component));

		break;

		// 描画コンポーネントの場合
	case Category::Renderer:
		m_pScene->m_rendererManager->RemoveRenderer(static_cast<RendererBase*>(component));
		break;
	}
}
