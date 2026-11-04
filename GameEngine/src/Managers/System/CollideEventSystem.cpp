/**
 * @file    CollideEventSystem.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/26
 */

#include "pch.h"
#include "CollideEventSystem.h"

#include "Physics/HitContact.h"

#include "Components/World/Collider/3D/ColliderBase.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"

namespace RHEngine
{
	void CollideEventSystem::RemoveCollider(ColliderBase* pCollider)
	{
		std::vector<decltype(m_prevMap.begin())> its;

		auto current = m_prevMap.begin();

		while (current != m_prevMap.end())
		{
			// 配列に含まれているなら削除する
			auto it = std::find_if(current, m_prevMap.end(), [&](std::pair<ObjectPair, HitContact> contact) -> bool
				{
					if (contact.second.otherCol == pCollider || contact.second.ownCol == pCollider)
					{
						return true;
					}
					return false;
				});

			// 見つかった場合
			if (it != m_prevMap.end())
			{
				// 配列にイテレータを追加
				its.push_back(it);

				// 見つかった次のイテレータからを次の探索対象とする
				current = std::next(it);
			}
			else
			{
				break;
			}
		}

		// 見つかったイテレータを配列から削除していく
		for (auto& it : its)
		{
			m_prevMap.erase(it);
		}
	}

	void CollideEventSystem::RemoveCollider2D(ColliderBase2D* pCollider)
	{
		std::vector<decltype(m_prevMap2D.begin())> its;

		auto current = m_prevMap2D.begin();

		while (current != m_prevMap2D.end())
		{
			// 配列に含まれているなら削除する
			auto it = std::find_if(current, m_prevMap2D.end(), [&](std::pair<ObjectPair2D, HitContact2D> contact) -> bool
				{
					if (contact.second.otherCol == pCollider || contact.second.ownCol == pCollider)
					{
						return true;
					}
					return false;
				});

			// 見つかった場合
			if (it != m_prevMap2D.end())
			{
				// 配列にイテレータを追加
				its.push_back(it);

				// 見つかった次のイテレータからを次の探索対象とする
				current = std::next(it);
			}
			else
			{
				break;
			}
		}

		// 見つかったイテレータを配列から削除していく
		for (auto& it : its)
		{
			m_prevMap2D.erase(it);
		}
	}

	void CollideEventSystem::CallCollideFunctions(std::unordered_map<ObjectPair, HitContact, ObjectPairHash>& nowMap)
	{
		// Enter & Stay
		for (auto& pair : nowMap)
		{
			if (m_prevMap.count(pair.first) == 0)
				OnEnter(pair.second);
			else
				OnStay(pair.second);
		}

		// Exit
		for (auto& pair : m_prevMap)
		{
			if (nowMap.count(pair.first) == 0)
				if (pair.first.a && pair.first.b)
					OnExit(pair.second);
		}

		m_prevMap = nowMap;
	}

	void CollideEventSystem::CallCollideFunctions2D(std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& nowMap)
	{
		// Enter & Stay
		for (auto& pair : nowMap)
		{
			if (m_prevMap2D.count(pair.first) == 0)
				OnEnter2D(pair.second);
			else
				OnStay2D(pair.second);
		}

		// Exit
		for (auto& pair : m_prevMap2D)
		{
			if (nowMap.count(pair.first) == 0)
				if (pair.first.a && pair.first.b)
					OnExit2D(pair.second);
		}

		m_prevMap2D = nowMap;
	}

	void CollideEventSystem::OnEnter(HitContact& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerEnter(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerEnter(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionEnter(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionEnter(hit);
		}
	}

	void CollideEventSystem::OnStay(HitContact& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerStay(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerStay(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionStay(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionStay(hit);
		}
	}

	void CollideEventSystem::OnExit(HitContact& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerExit(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerExit(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionExit(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionExit(hit);
		}
	}

	void CollideEventSystem::OnEnter2D(HitContact2D& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerEnter2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerEnter2D(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionEnter2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionEnter2D(hit);
		}
	}

	void CollideEventSystem::OnStay2D(HitContact2D& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerStay2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerStay2D(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionStay2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionStay2D(hit);
		}
	}

	void CollideEventSystem::OnExit2D(HitContact2D& contact)
	{
		if (contact.isTrigger)
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnTriggerExit2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnTriggerExit2D(hit);
		}
		else
		{
			if (contact.ownCol) contact.ownCol->GetReceiver()->OnCollisionExit2D(contact);
			auto hit = contact.Inverse();
			if (contact.otherCol) contact.otherCol->GetReceiver()->OnCollisionExit2D(hit);
		}
	}
}	// namespace RHEngine
