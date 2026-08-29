//====================================================//
// ファイル名  : ComponentContainer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要       : コンポーネントを保持するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameObject/ComponentContainer.h"

#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
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
} // namespace REngine
