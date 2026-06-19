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
#include "ComponentContainer.h"

#include "GameLib/Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// シーンにコンポーネントを登録する
void ComponentContainer::RegisterComponentToScene(BaseComponent* component)
{
    if (m_pScene && component) m_pScene->RegisterComponent(component);
}

// シーンからコンポーネントを登録解除する
void ComponentContainer::UnRegisterComponentToScene(BaseComponent* component)
{
    if (m_pScene && component) m_pScene->UnRegsisterComponent(component);
}

// 予約済みコンポーネントを全て登録、追加する
void ComponentContainer::RegisterComponents()
{
    // その他コンポーネント
    for (auto& component : m_pReservesAdd)
    {
        // シーンに登録
        m_pScene->RegisterComponent(component.get());

        // Awake呼び出し
        component->Awake();

        // リストに追加
        m_pComponents.push_back(std::move(component));
    }

    m_pReservesAdd.clear();
}

void ComponentContainer::RemoveRegistered()
{
    // その他コンポーネント
    for (auto& component : m_pDestroyReserves)
    {
        // コンポーネントをマネージャーから削除
        if (m_pScene) UnRegisterComponentToScene(component);

        // 探索
        auto it = std::find_if(
            m_pComponents.begin(), m_pComponents.end(),
            [&component](const std::unique_ptr<BaseComponent>& ptr) {
                return ptr.get() == component;
            });

        // 見つかれば削除
        if (it != m_pComponents.end())
        {
            m_pComponents.erase(it);
        }
    }

    m_pDestroyReserves.clear();
}
