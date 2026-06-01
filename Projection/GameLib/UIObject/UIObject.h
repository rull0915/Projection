//====================================================//
// ファイル名   : UIObject.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 :
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "UIComponents/RectTransform/RectTransform.h"
#include "UIComponents/UIComponents.h"

//====================================================//
// 前方宣言
//====================================================//
class Canvas;

//====================================================//
// クラス宣言
//====================================================//
class UIObject
{
private:
    // ---- コンポーネント ---- //

    // トランスフォーム
    std::unique_ptr<RectTransform> m_pTransform;

    // その他コンポーネント
    std::vector<std::unique_ptr<BaseUIComponent>> m_pReserves;
    std::vector<BaseUIComponent*> m_pDestroyReserves;
    std::vector<std::unique_ptr<BaseUIComponent>> m_pComponents;

    // アクティブフラグ　
    bool m_isActive;

    // 削除フラグ
    bool m_isDead;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------

    UIObject();
    virtual ~UIObject() {};

public:
    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------
	bool IsActive() const { return m_isActive; }
	bool IsDead() const { return m_isDead; }

    //-----------------------------------------------------
	// セッター
    //-----------------------------------------------------
	void SetActive(bool f) { m_isActive = f; }
	void SetDead(bool f) { m_isDead = f; }

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // キャンバスを取得する関数
    Canvas* GetCanvas();

    // ------------ コンポーネント操作 ----------------- //

    // 全コンポーネントの更新関数
    void ComponentsUpdate(float elapsedTime) 
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
            component->Update(elapsedTime);
        }
    }
    // コンポーネントを追加する関数
    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    T* AddComponent(Args&&... args);

    // コンポーネントを1つ取得する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    T* GetComponent();

    // コンポーネントがあるか調べる関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    bool HasComponent();

    // コンポーネントをすべて取得する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    std::vector<T*> GetComponents();

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    void GetComponents(std::vector<T*>& array);

    // コンポーネントを全て削除する関数
    void RemoveComponents();

    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseUIComponent, T>::value>>
    void RemoveComponent(T* component);

    // 予約済みのコンポーネントを登録する関数
    void RegisterComponents(){
        // その他コンポーネント
        for (auto& component : m_pReserves)
        {
            component->Awake();
    
            m_pComponents.push_back(std::move(component));
        }  
        m_pReserves.clear();
    }

    // 削除予約済みコンポーネントを削除する関数
    void RemoveRegistered()
    {
        // コンポーネント
        for (auto& component : m_pDestroyReserves)
        {
            // 配列から削除
            auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(),
                [component](const std::unique_ptr<BaseUIComponent>& ptr) {
                    return ptr.get() == component;
                });

            if (it != m_pComponents.end())
            {
                m_pComponents.erase(it);
            }
        }
    }
};

/// <summary>
/// コンポーネントを追加する関数
/// </summary>
/// <typeparam name="T">コンポーネントの型</typeparam>
/// <param name="...args">コンポーネントの引数</param>
/// <returns></returns>
template<typename T, typename... Args, typename>
T* UIObject::AddComponent(Args&&... args)
{
    T* add = nullptr;

    if constexpr (T::TYPE_ID == UIComponentID::RectTransform) return nullptr;

    else
    {
        // ユニークポインタを生成
        auto comp = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* ptr = comp.get();

        // 配列に追加
        m_pReserves.push_back(std::move(comp));

        add = ptr;
    }

    // 追加したコンポーネントを返す
    return add;
}

/// <summary>
/// コンポーネントを1つ取得する関数
/// </summary>
/// <typeparam name="T">取得したい型</typeparam>
/// <returns>コンポーネントのポインタ</returns>
template<typename T, typename>
inline T* UIObject::GetComponent()
{
    // TransformやRigidBodyなどは専用ポインタから即リターン
    if constexpr (T::TYPE_ID == UIComponentID::RectTransform) return m_pTransform.get();

    else {
        constexpr uint16_t mask = T::IS_MAIN ? 0xFF00 : 0xFFFF;

        // それ以外は ID 比較でループ
        for (auto& comp : m_pComponents)
        {
            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
            {
                return static_cast<T*>(comp.get());
            }
        }
        // 予約リストをチェック
        for (auto& comp : m_pReserves)
        {
            // コンポーネントが持つ生のIDを、同じマスクで切り落として比較する
            if ((MaskOperation(comp->GetID(), mask)) == (MaskOperation(T::TYPE_ID, mask)))
            {
                return static_cast<T*>(comp.get());
            }
        }

        return nullptr;
    }
}

template<typename T, typename>
inline bool UIObject::HasComponent()
{
    return GetComponent<T>() != nullptr;
}

/// <summary>
/// 全てのコンポーネントを取得する関数
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<typename T, typename>
inline std::vector<T*> UIObject::GetComponents()
{
    std::vector<T*> list;

    GetComponents(list);

    return list;
}

template<typename T, typename>
inline void UIObject::GetComponents(std::vector<T*>& array)
{
    // 配列の初期化
    array.clear();

    // 1. TransformやRigidBodyなどは専用ポインタから即追加
    if constexpr (T::TYPE_ID == UIComponentID::RectTransform) array.push_back(m_pTransform.get());

    else
    {
        constexpr uint16_t mask = T::IS_MAIN ? 0xFF00 : 0xFFFF;

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
        for (auto& comp : m_pReserves)
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

template<typename T, typename>
inline void UIObject::RemoveComponent(T* component)
{
    m_pDestroyReserves.push_back(component);
}
