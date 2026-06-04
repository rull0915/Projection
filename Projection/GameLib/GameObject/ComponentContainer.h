//====================================================//
// ファイル名   : ComponentContainer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要 : コンポーネントを保持するクラス
//
// 更新履歴 :
// 2026/05/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <vector>

#include "Components/Components.h"

//====================================================//
// 前方宣言
//====================================================//
class Scene;
class IComponentOwner;

//====================================================//
// クラス宣言
//====================================================//
class ComponentContainer
{
private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 自身を持つシーンのポインタ
    Scene* m_pScene;
    
    // トランスフォーム
    std::unique_ptr<Transform> m_pTransform;

    // その他コンポーネント
    std::vector<std::unique_ptr<BaseComponent>> m_pReservesAdd;
    std::vector<BaseComponent*> m_pDestroyReserves;
    std::vector<std::unique_ptr<BaseComponent>> m_pComponents;

    IComponentOwner* m_pOwner;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ComponentContainer(IComponentOwner* owner)
        : m_pTransform{ std::make_unique<Transform>(owner) }
        , m_pReservesAdd{}
        , m_pDestroyReserves{}
        , m_pComponents{}
        , m_pScene{ nullptr }
        , m_pOwner{ owner }
    {
        m_pTransform->UpdateCache();
    }
    ~ComponentContainer() = default;

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------
    void SetScene(Scene* scene) { m_pScene = scene; }

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 全コンポーネントを更新する関数
    void UpdateComponets(const GameTimer& gameTimer)
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
    void LateUpdateComponents(const GameTimer& gameTimer)
    {
        for (auto& component : m_pComponents)
        {
            if (!component->IsActive()) continue;

            // 遅延更新処理
            component->LateUpdate(gameTimer);
        }
    }

    // ===================== Component操作関数 ===================== //

    // ---------- Get ---------- //

    BaseComponent* Get(ComponentID id, bool isMain)
    {
        if (id == ComponentID::Transform) return m_pTransform.get();

        else {
            uint16_t mask = isMain ? MAIN_MASK : SUB_MASK;

            // それ以外は ID 比較でループ
            for (auto& comp : m_pComponents)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
                {
                    return comp.get();
                }
            }
            // 予約リストをチェック
            for (auto& comp : m_pReservesAdd)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
                {
                    return comp.get();
                }
            }

            return nullptr;
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    T* Get()
    {
        return static_cast<T*>(Get(T::TYPE_ID, T::IS_MAIN));
    }

    std::vector<BaseComponent*> Gets(ComponentID id, bool isMain) 
    {
        std::vector<BaseComponent*> components;
        Gets(id, isMain, components);
        return components;
    }

    void Gets(ComponentID id, bool isMain, std::vector<BaseComponent*>& array)
    {
        // 配列の初期化
        array.clear();

        // 1. TransformやRigidBodyなどは専用ポインタから即追加
        if (id == ComponentID::Transform) array.push_back(m_pTransform.get());

        else
        {
            uint16_t mask = isMain ? MAIN_MASK : SUB_MASK;

            // 2. それ以外は ID 比較でループ
            for (auto& comp : m_pComponents)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
                {
                    array.push_back(comp.get());
                }
            }
            // 予約リストをチェック
            for (auto& comp : m_pReservesAdd)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
                {
                    array.push_back(comp.get());
                }
            }
        }

        return;
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    std::vector<T*> Gets()
    {
        std::vector<T*> array;

        Gets<T>(array);

        return array;
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void Gets(std::vector<T*>& array)
    {
        // 配列の初期化
        array.clear();

        // 1. TransformやRigidBodyなどは専用ポインタから即追加
        if constexpr (T::TYPE_ID == ComponentID::Transform) array.push_back(m_pTransform.get());

        else
        {
            constexpr uint16_t mask = T::IS_MAIN ? MAIN_MASK : SUB_MASK;

            // 2. それ以外は ID 比較でループ
            for (auto& comp : m_pComponents)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
                {
                    array.push_back(static_cast<T*>(comp.get()));
                }
            }
            // 予約リストをチェック
            for (auto& comp : m_pReservesAdd)
            {
                // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
                if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
                {
                    array.push_back(static_cast<T*>(comp.get()));
                }
            }
        }

        return;
    }

    std::vector<std::unique_ptr<BaseComponent>>& GetAll()
    {
        return m_pComponents;
    }

    // ----------- Add ----------- //

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    T* Add()
    {
        T* add = nullptr;

        if constexpr (T::TYPE_ID == ComponentID::Transform) return nullptr;

        else
        {
            // ユニークポインタを生成
            auto comp = std::make_unique<T>(m_pOwner);
            T* ptr = comp.get();

            // 配列に追加
            m_pReservesAdd.push_back(std::move(comp));

            add = ptr;
        }

        // 追加したコンポーネントを返す
        return add;
    }

    // ----------- Has ----------- //

    bool Has(ComponentID id, bool isMain)
    {
        return Get(id, isMain) != nullptr;
    }
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>

    bool Has()
    {
        return Has(T::TYPE_ID, T::IS_MAIN);
    }

    // ---------- Remove ---------- //

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void Remove(T* component)
    {
        m_pDestroyReserves.push_back(component);
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void Remove()
    {
        for (auto& comp : m_pComponents)
        {
            constexpr uint16_t mask = T::IS_MAIN ? MAIN_MASK : SUB_MASK;

            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
            {
                Remove(comp.get());
            }
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void Removes()
    {
        for (auto& comp : m_pComponents)
        {
            constexpr uint16_t mask = T::IS_MAIN ? MAIN_MASK : SUB_MASK;

            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
            {
                Remove(comp.get());
            }
        }
    }

    void Remove(ComponentID id, bool isMain)
    {
        for (auto& comp : m_pComponents)
        {
            uint16_t mask = isMain ? MAIN_MASK : SUB_MASK;

            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
            {
                Remove(comp.get());

                return;
            }
        }
    }

    void Removes(ComponentID id, bool isMain)
    {
        for (auto& comp : m_pComponents)
        {
            uint16_t mask = isMain ? MAIN_MASK : SUB_MASK;

            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(id, mask)))
            {
                Remove(comp.get());
            }
        }
    }

    void AllRemove()
    {
        for (auto& comp : m_pComponents)
        {
            Remove(comp.get());
        }
    }

    // 予約済みのコンポーネントを登録する関数
    void RegisterComponents();

    // 削除予約済みのコンポーネントを削除する関数
    void RemoveRegistered();

private:

    // シーンにコンポーネントを登録する関数
    void RegisterComponentToScene(BaseComponent* component);

    // シーンからコンポーネントを登録解除する関数
    void UnRegisterComponentToScene(BaseComponent* component);
};
