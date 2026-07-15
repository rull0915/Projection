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
#include "Scene/ComponentRegister.h"

#include "Scene/Scene.h"
#include "Components/ComponentBase.h"

#include "Scene/UpdatePipeline.h"
#include "System/TypeIdGenerator.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class RigidBody;
	class RigidBody2D;

	//====================================================//
	// 関数の実体宣言
	//====================================================//

	ComponentRegister::ComponentRegister(UpdatePipeline* pipeline)
		: m_pPipeline{ pipeline }
		, m_componentsMap{}
		, m_categoriesMap{}
		, m_listeners{}
	{}

	void ComponentRegister::RegisterComponent(ComponentBase* component)
	{
		// IDを取得
		unsigned int id = component->GetID();

		// カテゴリを取得
		ComponentCategory category = component->GetCategory();

		// リストに追加
		m_componentsMap[id].push_back(component);
		m_categoriesMap[category].push_back(component);

		// 読み取りコンポーネントなら追加
		if (IComponentListener* listener = dynamic_cast<IComponentListener*>(component)) m_listeners.push_back(listener);
		// デバッグ描画コンポーネントなら追加
		if (IDebugRenderable* render = dynamic_cast<IDebugRenderable*>(component)) m_debugRenders.push_back(render);

		// カテゴリで分類
		switch (category)
		{
			// カテゴリに分類されないコンポーネントの場合
		case Category::Original:

			// IDで識別

			// RigidBodyの場合
			if (id == TypeIDGenerator::GetID<RigidBody>()) {
				m_pPipeline->m_physicsManager->AddRigidBody(static_cast<RigidBody*>(component));
				break;
			}

			// RigidBody2Dの場合
			if (id == TypeIDGenerator::GetID<RigidBody2D>()) {
				m_pPipeline->m_physicsManager2D->AddRigidBody(static_cast<RigidBody2D*>(component));
				break;
			}

			// AudioSourceの場合
			if (id == TypeIDGenerator::GetID<AudioSource>()) {
				m_pPipeline->m_soundManager->AddAudioSource(static_cast<AudioSource*>(component));
				break;
			}

			// AudioListenerの場合
			if (id == TypeIDGenerator::GetID<AudioListener>()) {
				m_pPipeline->m_soundManager->SetListener(static_cast<AudioListener*>(component));
				break;
			}

			break;

			// カメラコンポーネントの場合
		case Category::Camera:
			m_pPipeline->m_cameraManager->SetMainCamera(static_cast<CameraBase*>(component));
			break;

			// 3Dコライダーの場合
		case Category::Collider:
			m_pPipeline->m_physicsManager->GetCollideManager()->AddCollide(static_cast<ColliderBase*>(component));
			break;

			// 2Dコライダーの場合
		case Category::Collider2D:
			m_pPipeline->m_physicsManager2D->GetCollideManager()->AddCollide(static_cast<ColliderBase2D*>(component));
			break;

			// 描画コンポーネントの場合
		case Category::Renderer:
			m_pPipeline->m_rendererManager->AddRenderer(static_cast<RendererBase*>(component));
			break;
		}

		// 読み取りコンポーネントに通知
		for (auto& listener : m_listeners) listener->OnComponentAdded(component);
	}

	void ComponentRegister::UnRegisterComponent(ComponentBase* component)
	{
		// IDを取得
		unsigned int id = component->GetID();

		// カテゴリを取得
		ComponentCategory category = component->GetCategory();

		// リストから削除
		auto& list = m_componentsMap[id];

		// 同じポインタがあれば削除
		std::erase_if(list, [&](const ComponentBase* c) { return c == component; });

		// リストが空になった場合マップから削除
		if (list.empty()) m_componentsMap.erase(id);

		// カテゴリ版リストからも削除
		auto& categoryList = m_categoriesMap[category];

		// 同じポインタがあれば削除
		std::erase_if(categoryList, [&](const ComponentBase* c) { return c == component; });

		// リストが空になった場合マップから削除
		if (categoryList.empty()) m_categoriesMap.erase(category);

		// 読み取りコンポーネントなら削除
		if (IComponentListener* listener = dynamic_cast<IComponentListener*>(component)) { std::erase(m_listeners, listener); }
		// デバッグ描画コンポーネントなら削除
		if (IDebugRenderable* debugRender = dynamic_cast<IDebugRenderable*>(component)) { std::erase(m_debugRenders, debugRender); }

		// カテゴリで分類
		switch (category)
		{
			// カテゴリに分類されないコンポーネントの場合
		case Category::Original:

			// IDで識別

			// RigidBodyの場合
			if (id == TypeIDGenerator::GetID<RigidBody>()) {
				m_pPipeline->m_physicsManager->RemoveRigidBody(static_cast<RigidBody*>(component));
				break;
			}

			// RigidBody2Dの場合
			if (id == TypeIDGenerator::GetID<RigidBody2D>()) {
				m_pPipeline->m_physicsManager2D->RemoveRigidBody(static_cast<RigidBody2D*>(component));
				break;
			}

			// AudioSourceの場合
			if (id == TypeIDGenerator::GetID<AudioSource>()) {
				m_pPipeline->m_soundManager->RemoveAudioSource(static_cast<AudioSource*>(component));
				break;
			}

			// AudioListenerの場合
			if (id == TypeIDGenerator::GetID<AudioListener>()) {
				m_pPipeline->m_soundManager->RemoveListener(static_cast<AudioListener*>(component));
				break;
			}

			break;

			// カメラコンポーネントの場合
		case Category::Camera:
			m_pPipeline->m_cameraManager->UnSetMainCamera(static_cast<CameraBase*>(component));
			break;

			// 3Dコライダーの場合
		case Category::Collider:
			m_pPipeline->m_physicsManager->GetCollideManager()->RemoveCollide(static_cast<ColliderBase*>(component));
			m_pPipeline->m_colEvent->RemoveCollider(static_cast<ColliderBase*>(component));
			break;

			// 2Dコライダーの場合
		case Category::Collider2D:
			m_pPipeline->m_physicsManager2D->GetCollideManager()->RemoveCollide(static_cast<ColliderBase2D*>(component));
			m_pPipeline->m_colEvent->RemoveCollider2D(static_cast<ColliderBase2D*>(component));

			break;

			// 描画コンポーネントの場合
		case Category::Renderer:
			m_pPipeline->m_rendererManager->RemoveRenderer(static_cast<RendererBase*>(component));
			break;
		}

		// 読み取りコンポーネントに通知
		for (auto& listener : m_listeners) listener->OnComponentRemoved(component);
	}
}	// namespace REngine
