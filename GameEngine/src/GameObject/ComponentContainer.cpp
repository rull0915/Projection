/**
 * @file    ComponentContainer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/29
 */

#include "pch.h"
#include "GameObject/ComponentContainer.h"

#include "Scene/Scene.h"

namespace RHEngine
{
	ComponentContainer::ComponentContainer(IComponentOwner* pOwner)
		: m_pTransform{ nullptr }
		, m_addReserves{}
		, m_pDestroyReserves{}
		, m_pComponents{}
		, m_pScene{ nullptr }
		, m_pOwner{ pOwner }
	{}

	void ComponentContainer::AwakeComponets()
	{
		for (auto& component : m_addReserves)
		{
			// 更新処理
			component->Awake();

			// Activeなら
			if (component->IsActive())
			{
				component->OnEnable();
			}
		}
	}

	void ComponentContainer::UpdateComponents(const GameTimer& gameTimer)
	{
		for (auto& component : m_pComponents)
		{
			if (!component->IsActive()) continue;

			// 初回更新の時
			if (!component->IsStarted())
			{
				// Start関数呼び出し
				component->Start();
				component->SetStart();
			}

			// 更新処理
			component->Update(gameTimer);
		}
	}

	void ComponentContainer::LateUpdateComponents(const GameTimer& gameTimer)
	{
		for (auto& component : m_pComponents)
		{
			if (!component->IsActive()) continue;

			// 遅延更新処理
			component->LateUpdate(gameTimer);
		}
	}

	bool ComponentContainer::IsCorrect(ComponentSpace space) const
	{
		// 無効オブジェクトなら
		if (!IsValid()) return false;

		// Both
		if (space == ComponentSpace::Both) return true;

		// UI判定
		if (space == ComponentSpace::UI && IsUI()) return true;

		// World判定
		if (space == ComponentSpace::World && IsWorld()) return true;

		// 正しくない
		return false;
	}

	ComponentBase* ComponentContainer::Get(Component::TypeId id) const
	{
		// Transformは専用ポインタから早期リターン
		if (id == Transform::StaticTypeId())
		{
			return m_pTransform.get();
		}
		// RectTransformも同様
		else if (id == RectTransform::StaticTypeId())
		{
			return m_pRectTransform.get();
		}
		else
		{
			// それ以外はキャッシュから取得
			auto it = m_componentsCache.find(id);

			// 指定されたidの配列があり1つ以上コンポーネントがあれば
			if (it != m_componentsCache.end() && it->second.size() > 0)
			{
				return it->second[0];
			}
		}

		return nullptr;
	}

	const std::vector<ComponentBase*>& ComponentContainer::Gets(Component::TypeId id) const
	{
		// 存在しなかったとき用の空配列
		static const std::vector<ComponentBase*> empty{};

		// イテレータを取得
		auto it = m_componentsCache.find(id);

		return it != m_componentsCache.end() ? it->second : empty;
	}

	// シーンにコンポーネントを登録する
	void ComponentContainer::RegisterComponentToScene(ComponentBase* component)
	{
		if (m_pScene && component) m_pScene->RegisterComponent(component);
	}

	// シーンからコンポーネントを登録解除する
	void ComponentContainer::UnRegisterComponentToScene(ComponentBase* component)
	{
		if (m_pScene && component) m_pScene->UnRegsisterComponent(component);
	}

	void ComponentContainer::RegistComponentToCache(ComponentBase* pComponent)
	{
		// キャッシュリストを格納する配列を用意
		std::vector<Component::TypeId> ids;

		// 対応するIDの一覧を取得
		pComponent->CollectTypeIds(ids);

		// 全要素に自身を追加
		for (auto id : ids)
		{
			m_componentsCache[id].push_back(pComponent);
		}
	}

	void ComponentContainer::UnRegistComponentToCache(ComponentBase* component)
	{
		// キャッシュリストを格納する配列を用意
		std::vector<Component::TypeId> ids;

		// 対応するIDの一覧を取得
		component->CollectTypeIds(ids);

		// 全要素から自身を削除
		for (auto id : ids)
		{
			// リストのイテレータを取得
			auto listIt = m_componentsCache.find(id);

			// 存在しないなら次のリストへ
			if (listIt == m_componentsCache.end())
				continue;

			// イテレータから本体を取得
			auto& list = listIt->second;

			// 自身が格納されているイテレータを取得
			auto it = std::find(list.begin(), list.end(), component);

			// 見つかれば
			if (it != list.end())
			{
				// 削除
				list.erase(it);
			}
		}
	}

	void ComponentContainer::Remove(Component::TypeId id)
	{
		// リストを取得
		auto it = m_componentsCache.find(id);

		// あれば
		if (it != m_componentsCache.end() && it->second.size() > 0)
		{
			// 最初の要素を削除リストに追加
			m_pDestroyReserves.insert(it->second[0]);
		}
	}

	void ComponentContainer::Removes(Component::TypeId id)
	{
		// リストを取得
		auto it = m_componentsCache.find(id);

		// あれば
		if (it != m_componentsCache.end())
		{
			// 全て削除リストに追加
			for (auto& comp : it->second)
			{
				m_pDestroyReserves.insert(comp);
			}
		}
	}

	// 予約済みコンポーネントを全て登録、追加する
	void ComponentContainer::RegisterComponents()
	{
		// その他コンポーネント
		for (auto& component : m_addReserves)
		{
			// リストに追加
			m_pComponents.push_back(std::move(component));
		}

		m_addReserves.clear();
	}

	void ComponentContainer::RemoveRegistered()
	{
		// その他コンポーネント
		for (auto& component : m_pDestroyReserves)
		{
			// Activeなら
			if (component->IsActive())
			{
				component->OnDisable();
			}

			// 削除時処理を呼ぶ
			component->OnDestroy();

			// コンポーネントをマネージャーから削除
			if (m_pScene) UnRegisterComponentToScene(component);

			// コンポーネントをキャッシュから削除
			UnRegistComponentToCache(component);

			// 目的のポインタと一致するイテレータを探索
			auto it = std::find_if(
				m_pComponents.begin(), m_pComponents.end(),
				[&component](const std::unique_ptr<ComponentBase>& ptr) {
					return ptr.get() == component;
				});

			// 見つかれば削除
			if (it != m_pComponents.end())
			{
				m_pComponents.erase(it);
			}
			else
			{
				// 予約リストから探索
				auto it = std::find_if(
					m_addReserves.begin(), m_addReserves.end(),
					[&component](const std::unique_ptr<ComponentBase>& ptr) {
						return ptr.get() == component;
					});

				// 見つかれば削除
				if (it != m_addReserves.end())
				{
					m_addReserves.erase(it);
				}
			}
		}

		m_pDestroyReserves.clear();
	}
} // namespace RHEngine
