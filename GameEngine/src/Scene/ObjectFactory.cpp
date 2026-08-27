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
#include "Managers/UI/UIManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	ObjectFactory::ObjectFactory(Scene* pScene)
		: m_pScene{ pScene }
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

		// 作成したポインタを返す
		return pObj;
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

		// 作成したポインタを返す
		return pObj;
	}
}	// namespace REngine
