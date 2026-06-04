//====================================================//
// ファイル名  : ObjectManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/07
//
// 概要        :  GameObjectを管理するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ObjectManager.h"

// 2Dマネージャー
#include "2DManagers/Collider/CollideManager2D.h"
#include "2DManagers/PhysicsManager2D.h"

// 3Dマネージャー
#include "3DManagers/Collider/CollideManager.h"
#include "3DManagers/PhysicsManager.h"
#include "System/CollideEventSystem.h"

// その他マネージャー
#include "CameraManager.h"

#include "GameLib/Resources/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
ObjectManager::ObjectManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
ObjectManager::~ObjectManager()
{
}

/// <summary>
/// 更新関数
/// </summary>
void ObjectManager::Update(const GameTimer& gameTimer)
{
	// 予約されているオブジェクトを追加
	AddReservedObject();

	// 全オブジェクトのコンポーネントを追加
	for (auto& object : m_objects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->GetComponentContainer().RegisterComponents();
	}

	// 全オブジェクトの更新関数呼び出し
	for (auto& object : m_objects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 更新処理
		object->BaseUpdate(gameTimer);
	}
}

/// <summary>
/// 遅延更新関数
/// </summary>
void ObjectManager::LateUpdate(const GameTimer& gameTimer)
{
	// 全オブジェクトの遅延更新を呼び出す
	for (auto& object : m_objects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->BaseLateUpdate(gameTimer);
	}
}

void ObjectManager::RemoveDeadComponent()
{
	// 全オブジェクトの予約済みコンポーネントを削除
	for (auto& object : m_objects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->GetComponentContainer().RemoveRegistered();
	}
}

/// <summary>
/// 描画関数
/// </summary>
void ObjectManager::Render(Renderer& renderer)
{
	// 全オブジェクトの描画
	for (auto& object : m_objects)
	{
		// 基底クラスの描画処理
		object->BaseRender(renderer);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void ObjectManager::Finalize()
{
	AllDestroy();
	RemoveDeadObject();
}

void ObjectManager::AllReflectCache()
{
	// 衝突判定後の値の更新
	for (auto& object : m_objects)
	{
		object->GetComponent<Transform>()->ReflectCache();	// Transform
	}
}

void ObjectManager::AllDestroy()
{
	// 管理リストを全て調べる
	for (auto& obj : m_objects)
	{
		obj->Destroy();
	}
}

/// <summary>
/// 予約済みオブジェクトを全て追加する関数
/// </summary>
void ObjectManager::AddReservedObject()
{
	// 予約リストを全て調べる
	for (auto& p : m_reservations)
	{
		// 管理リストへ追加
		m_objects.push_back(std::unique_ptr<GameObject>(p));
	}

	// 予約リストをリセット
	m_reservations.clear();
}

void ObjectManager::RemoveDeadObject()
{
	// 管理リストを全て調べる
	for (int i = 0; i < m_objects.size(); i++)
	{
		GameObject* obj = m_objects[i].get();

		// オブジェクトの死亡フラグがオンなら
		if (obj->IsDead())
		{
			obj->BaseFinalize();

			// リストから削除
			m_objects.erase(m_objects.begin() + i);

			i--;
		}
	}
}
