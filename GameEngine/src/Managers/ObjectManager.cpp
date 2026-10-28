/**
 * @file    ObjectManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/07
 */

#include "pch.h"
#include "Managers/ObjectManager.h"

namespace RHEngine
{
	ObjectManager::ObjectManager()
	{}

	ObjectManager::~ObjectManager()
	{}

	void ObjectManager::Update(const GameTimer& gameTimer, bool playing)
	{
		// 予約されているオブジェクトを追加
		AddReservedObject();

		// 全オブジェクトのアクティブ反映
		for (auto& pObject : m_objects)
		{
			pObject->ActiveReserve();
		}

		// 全オブジェクトのコンポーネントを追加
		for (auto& pObject : m_objects)
		{
			// アクティブチェック
			if (pObject->IsActive())
			{
				// Awakeの呼び出し
				if (playing)
				{
					pObject->GetComponentContainer().AwakeComponets();
				}
			}

			// 予約されたコンポーネントを追加
			pObject->GetComponentContainer().RegisterComponents();
		}

		// 全オブジェクトの更新関数呼び出し
		for (auto& pObject : m_objects)
		{
			// アクティブチェック
			if (!pObject->IsActive()) continue;

			// Start処理

			// 更新処理
			if (playing)
			{
				pObject->GetComponentContainer().UpdateComponents(gameTimer);
			}
		}
	}

	/// <summary>
	/// 遅延更新関数
	/// </summary>
	void ObjectManager::LateUpdate(const GameTimer& gameTimer, bool playing)
	{
		// 全オブジェクトの遅延更新を呼び出す
		for (auto& pObject : m_objects)
		{
			// アクティブチェック
			if (!pObject->IsActive()) continue;

			// 予約されたコンポーネントを追加
			if (playing)
				pObject->GetComponentContainer().LateUpdateComponents(gameTimer);
		}
	}

	void ObjectManager::RemoveDeadComponent()
	{
		// 全オブジェクトの予約済みコンポーネントを削除
		for (auto& pObject : m_objects)
		{
			// 予約されたコンポーネントを追加
			pObject->GetComponentContainer().RemoveRegistered();
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
		for (auto& pObject : m_objects)
		{
			if (auto* pTransform = pObject->GetComponent<Transform>()) pTransform->ReflectCache();	// Transform
		}
	}

	void ObjectManager::AllDestroy()
	{
		// 管理リストを全て調べる
		for (auto& pObject : m_objects)
		{
			// 削除
			pObject->Destroy();
		}
	}

	/// <summary>
	/// 予約済みオブジェクトを全て追加する関数
	/// </summary>
	void ObjectManager::AddReservedObject()
	{
		// 予約リストを全て調べる
		for (auto& pObject : m_reservations)
		{
			// 管理リストへ追加
			m_objects.push_back(std::move(pObject));
		}

		// 予約リストをリセット
		m_reservations.clear();
	}

	void ObjectManager::RemoveDeadObject()
	{
		// Deadフラグがオンのオブジェクトを削除
		std::erase_if(
			m_objects,
			[](const std::unique_ptr<GameObject>& object)
			{
				// フラグがオンなら
				if (object->IsDead())
				{
					// 終了処理
					object->Finalize();

					// true
					return true;
				}
				return false;
			}
		);
	}
}	// namespace RHEngine
