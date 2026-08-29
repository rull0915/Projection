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
#include "Components/ComponentBase.h"

#include "Scene/UpdatePipeline.h"

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
		, m_listeners{}
	{}

	void ComponentRegister::RegisterComponent(ComponentBase* component)
	{
		if (!component) return;

		// 対応IDリストを取得
		std::vector<Component::TypeId> ids;
		component->CollectTypeIds(ids);

		// リストに追加
		for (auto& id : ids) m_componentsMap[id].push_back(component);

		// 読み取りコンポーネントなら追加
		if (IComponentListener* listener = dynamic_cast<IComponentListener*>(component)) m_listeners.push_back(listener);
		// デバッグ描画コンポーネントなら追加
		if (IDebugRenderable* render = dynamic_cast<IDebugRenderable*>(component)) m_debugRenders.push_back(render);

		// IDで識別

		// Canvasの場合
		if (component->IsTypeOf(Canvas::StaticTypeId()))
		{
			m_pPipeline->m_uiManager->AddCanvas(static_cast<Canvas*>(component));
		}

		// RigidBodyの場合
		if (component->IsTypeOf(RigidBody::StaticTypeId()))
		{
			m_pPipeline->m_physicsManager->AddRigidBody(static_cast<RigidBody*>(component));
		}

		// RigidBody2Dの場合
		if (component->IsTypeOf(RigidBody2D::StaticTypeId())) {
			m_pPipeline->m_physicsManager2D->AddRigidBody(static_cast<RigidBody2D*>(component));
		}

		// AudioSourceの場合
		if (component->IsTypeOf(AudioSource::StaticTypeId())) {
			m_pPipeline->m_soundManager->AddAudioSource(static_cast<AudioSource*>(component));
		}

		// AudioListenerの場合
		if (component->IsTypeOf(AudioListener::StaticTypeId())) {
			m_pPipeline->m_soundManager->SetListener(static_cast<AudioListener*>(component));
		}

		// カメラコンポーネントの場合
		if (component->IsTypeOf(CameraBase::StaticTypeId())) {
			m_pPipeline->m_cameraManager->SetMainCamera(static_cast<CameraBase*>(component));
		}

		// 3Dコライダーの場合
		if (component->IsTypeOf(ColliderBase::StaticTypeId())) 
		{
			m_pPipeline->m_physicsManager->GetCollideManager()->AddCollide(static_cast<ColliderBase*>(component));
		}

		// 2Dコライダーの場合
		if (component->IsTypeOf(ColliderBase2D::StaticTypeId()))
		{
			m_pPipeline->m_physicsManager2D->GetCollideManager()->AddCollide(static_cast<ColliderBase2D*>(component));
		}

		// 描画コンポーネントの場合
		if (component->IsTypeOf(RendererBase::StaticTypeId()))
		{
			m_pPipeline->m_rendererManager->AddRenderer(static_cast<RendererBase*>(component));
		}

		// 読み取りコンポーネントに通知
		for (auto& listener : m_listeners) listener->OnComponentAdded(component);
	}

	void ComponentRegister::UnRegisterComponent(ComponentBase* component)
	{
		if (!component) return;

		// 対応IDリストを取得
		std::vector<Component::TypeId> ids;
		component->CollectTypeIds(ids);

		// リストから削除
		for (auto& id : ids)
		{
			auto& list = m_componentsMap[id];

			// 同じポインタがあれば削除
			std::erase_if(list, [&](const ComponentBase* c) { return c == component; });

			// リストが空になった場合マップから削除
			if (list.empty()) m_componentsMap.erase(id);
		}

		// 読み取りコンポーネントなら削除
		if (IComponentListener* listener = dynamic_cast<IComponentListener*>(component)) { std::erase(m_listeners, listener); }
		// デバッグ描画コンポーネントなら削除
		if (IDebugRenderable* debugRender = dynamic_cast<IDebugRenderable*>(component)) { std::erase(m_debugRenders, debugRender); }

		
		// IDで識別

		// Canvasの場合
		if (component->IsTypeOf(Canvas::StaticTypeId()))
		{
			m_pPipeline->m_uiManager->RemoveCanvas(static_cast<Canvas*>(component));
		}

		// RigidBodyの場合
		if (component->IsTypeOf(RigidBody::StaticTypeId()))
		{
			m_pPipeline->m_physicsManager->RemoveRigidBody(static_cast<RigidBody*>(component));
		}

		// RigidBody2Dの場合
		if (component->IsTypeOf(RigidBody2D::StaticTypeId())) {
			m_pPipeline->m_physicsManager2D->RemoveRigidBody(static_cast<RigidBody2D*>(component));
		}

		// AudioSourceの場合
		if (component->IsTypeOf(AudioSource::StaticTypeId())) {
			m_pPipeline->m_soundManager->RemoveAudioSource(static_cast<AudioSource*>(component));
		}

		// AudioListenerの場合
		if (component->IsTypeOf(AudioListener::StaticTypeId())) {
			m_pPipeline->m_soundManager->RemoveListener(static_cast<AudioListener*>(component));
		}

		// カメラコンポーネントの場合
		if (component->IsTypeOf(CameraBase::StaticTypeId())) {
			m_pPipeline->m_cameraManager->UnSetMainCamera(static_cast<CameraBase*>(component));
		}

		// 3Dコライダーの場合
		if (component->IsTypeOf(ColliderBase::StaticTypeId())) 
		{
			m_pPipeline->m_physicsManager->GetCollideManager()->RemoveCollide(static_cast<ColliderBase*>(component));
			m_pPipeline->m_colEvent->RemoveCollider(static_cast<ColliderBase*>(component));
		}

		// 2Dコライダーの場合
		if (component->IsTypeOf(ColliderBase2D::StaticTypeId()))
		{
			m_pPipeline->m_physicsManager2D->GetCollideManager()->RemoveCollide(static_cast<ColliderBase2D*>(component));
			m_pPipeline->m_colEvent->RemoveCollider2D(static_cast<ColliderBase2D*>(component));
		}

		// 描画コンポーネントの場合
		if (component->IsTypeOf(RendererBase::StaticTypeId()))
		{
			m_pPipeline->m_rendererManager->RemoveRenderer(static_cast<RendererBase*>(component));
		}

		// 読み取りコンポーネントに通知
		for (auto& listener : m_listeners) listener->OnComponentRemoved(component);
	}
}	// namespace REngine
