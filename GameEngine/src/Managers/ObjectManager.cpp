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
#include "Managers/ObjectManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectManager::ObjectManager()
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	ObjectManager::~ObjectManager()
	{}

	/// <summary>
	/// 更新関数
	/// </summary>
	void ObjectManager::Update(const GameTimer& gameTimer, bool playing)
	{
		// 予約されているオブジェクトを追加
		AddReservedObject();

		// 全オブジェクトのアクティブ反映
		for (auto& object : m_objects)
		{
			object->Reserve();
		}


		// 全オブジェクトのコンポーネントを追加
		for (auto& object : m_objects)
		{
			// アクティブチェック
			if (object->IsActive())
			{
				// Awakeの呼び出し
				if (playing || object->IsInvincible())
				{
					object->GetComponentContainer().AwakeComponets();
				}
			}

			// 予約されたコンポーネントを追加
			object->GetComponentContainer().RegisterComponents();
		}

		// 全オブジェクトの更新関数呼び出し
		for (auto& object : m_objects)
		{
			// アクティブチェック
			if (!object->IsActive()) continue;

			// Start処理

			// 更新処理
			if (playing || object->IsInvincible())
			{
				object->GetComponentContainer().UpdateComponents(gameTimer);
			}
		}
	}

	/// <summary>
	/// 遅延更新関数
	/// </summary>
	void ObjectManager::LateUpdate(const GameTimer& gameTimer, bool playing)
	{
		// 全オブジェクトの遅延更新を呼び出す
		for (auto& object : m_objects)
		{
			// アクティブチェック
			if (!object->IsActive()) continue;

			// 予約されたコンポーネントを追加
			if (playing || object->IsInvincible())
				object->GetComponentContainer().LateUpdateComponents(gameTimer);
		}
	}

	void ObjectManager::RemoveDeadComponent()
	{
		// 全オブジェクトの予約済みコンポーネントを削除
		for (auto& object : m_objects)
		{
			// 予約されたコンポーネントを追加
			object->GetComponentContainer().RemoveRegistered();
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
			// 削除可能なら
			if (!obj->IsInvincible())
			{
				// 削除
				obj->Destroy();
			}
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
			m_objects.push_back(std::move(p));
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
}	// namespace REngine
