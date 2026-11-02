/**
 * @file    ObjectFactory.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/12
 */

#include "pch.h"

#include "Scene/ObjectFactory.h"
#include "Scene/Scene.h"
#include "Managers/ObjectManager.h"
#include "System/UUIDRegistry.h"
#include "Editor/Loader/ObjectLoader.h"
#include "Assets/Types/PrefabImpl.h"
#include "Assets/Managers/AssetManager.h"

namespace RHEngine
{
	ObjectFactory::ObjectFactory(Scene* pScene, AssetManager& assetManager)
		: m_pScene{ pScene }
		, m_assetManager{ assetManager }
	{}

	GameObject* ObjectFactory::Generate(DirectX::SimpleMath::Vector3 position)
	{
		// ポインタを作成
		GameObject* pObj = new GameObject(GameObject::CreateToken{});

		// オブジェクトのシーンを設定
		pObj->SetScene(m_pScene);

		// オブジェクト管理クラスに追加
		m_pScene->GetObjectManager()->AddObject(pObj);

		// Transformを追加
		pObj->AddComponent<Transform>();

		// 位置を設定
		pObj->GetComponent<Transform>()->SetLocalPosition(position);

		// UUIDを生成
		pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

		// 作成したポインタを返す
		return pObj;
	}

	GameObject* ObjectFactory::Instantiate(Prefab* pPrefab) const
	{
		// Prefabがなければ何もしない
		if (!pPrefab) return nullptr;

		// jsonを取得
		ObjectLoader loader(m_pScene->GetAssetManager());

		// ポインタを作成
		GameObject* pObj = new GameObject(GameObject::CreateToken{});

		// オブジェクトのシーンを設定
		pObj->SetScene(m_pScene);

		// オブジェクト管理クラスに追加
		m_pScene->GetObjectManager()->AddObject(pObj);

		if (pPrefab->GetImpl()->IsWorldObject())
		{
			// Transformを追加
			pObj->AddComponent<Transform>();
		}
		else
		{
			// RectTransformを追加
			pObj->AddComponent<RectTransform>();
		}

		// UUIDを生成
		pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

		// jsonからロード
		loader.LoadObject(pPrefab->GetImpl()->GetJson(), pObj, m_pScene);

		return pObj;
	}

	GameObject* ObjectFactory::Instantiate(Handle<Prefab> prefab) const
	{
		// Prefab本体を取得
		Prefab* pPrefab = m_assetManager.Get(prefab);

		return Instantiate(pPrefab);
	}

	GameObject* ObjectFactory::GenerateUI()
	{
		// ポインタを作成
		GameObject* pObj = new GameObject(GameObject::CreateToken{});

		// オブジェクトのシーンを設定
		pObj->SetScene(m_pScene);

		// オブジェクト管理クラスに追加
		m_pScene->GetObjectManager()->AddObject(pObj);

		// RectTransformを追加
		pObj->AddComponent<RectTransform>();

		// UUIDを生成
		pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

		// 作成したポインタを返す
		return pObj;
	}
}	// namespace RHEngine
