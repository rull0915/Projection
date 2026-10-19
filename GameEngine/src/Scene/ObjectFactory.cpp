//====================================================//
// ファイル名  : ObjectFactory.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要       : オブジェクトの生成を行うクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#include "Scene/ObjectFactory.h"
#include "Scene/Scene.h"
#include "Managers/ObjectManager.h"
#include "System/UUIDRegistry.h"
#include "Editor/Loader/ObjectLoader.h"
#include "Assets/Types/PrefabImpl.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	ObjectFactory::ObjectFactory(Scene* pScene, AssetManager& assetManager)
		: m_pScene{ pScene }
		, m_assetManager{ assetManager }
	{}

	// ゲームオブジェクトを生成する関数
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

	GameObject* ObjectFactory::Instantiate(Prefab* prefab) const
	{
		// prefabがなければ何もしない
		if (!prefab) return nullptr;

		// jsonを取得
		ObjectLoader loader(m_pScene->GetAssetManager());

		// ポインタを作成
		GameObject* pObj = new GameObject(GameObject::CreateToken{});

		// オブジェクトのシーンを設定
		pObj->SetScene(m_pScene);

		// オブジェクト管理クラスに追加
		m_pScene->GetObjectManager()->AddObject(pObj);

		if (prefab->GetImpl()->IsWorldObject())
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
		loader.LoadObject(prefab->GetImpl()->GetJson(), pObj, m_pScene);

		return pObj;
	}

	GameObject* ObjectFactory::Instantiate(Handle<Prefab> prefab) const
	{
		// Prefab本体を取得
		Prefab* p = m_assetManager.Get(prefab);

		return Instantiate(prefab);
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
