//====================================================//
// ファイル名   : GameObject.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/19
//
// 概要 : ゲームオブジェクトクラスです
//
// 更新履歴 :
// 2026/03/19 新規作成
// 2026/05/28 final指定に変更し、コンポーネントを保持するだけの器という設計に変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <type_traits>

#include "Interface/IColliderReceiver.h"
#include "Interface/IComponentOwner.h"

#include "ComponentContainer.h"

//====================================================//
// 前方宣言
//====================================================//

class Scene;
class Renderer;

//====================================================//
// クラス宣言
//====================================================//
class GameObject final : public IComponentOwner, public IColliderReceiver
{
    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    
private:
    // コンポーネント
    ComponentContainer m_components;

    // 自身を持つシーンのポインタ
    Scene* m_pScene;

    // アクティブフラグ　
    bool m_isActive;

    // 削除フラグ
    bool m_isDead;

public:
    // タグ
    std::string m_tag;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    GameObject();

    void BaseUpdate(float elapsedTime);

    void BaseLateUpdate(float elapsedTime);

    void BaseRender(Renderer& renderer);

    void BaseFinalize();

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------
    bool IsActive() const { return m_isActive; }
    
    bool IsDead() const { return m_isDead; }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------
    void SetActive(bool value) { m_isActive = value; }

    void SetScene(Scene* scene) 
    {
        m_pScene = scene; 
        m_components.SetScene(scene);
    }

    void Destroy() override { m_isDead = true; }

private:
    // コライダーのデバッグ描画をする関数
    void CollidersDebugDraw(Renderer& renderer, int color, bool drawAABB = false);

public:
    // 衝突関連
#pragma region CollisionEvent

    // 衝突時に呼び出される関数

    // 3D
    void BaseOnCollisionEnter(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionEnter(contact);   // コンポーネントに通知
    };
    void BaseOnCollisionStay(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionStay(contact);   // コンポーネントに通知
    };
    void BaseOnCollisionExit(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionExit(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerEnter(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerEnter(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerStay(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerStay(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerExit(HitContact& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerExit(contact);   // コンポーネントに通知
    };

    // 2D
    void BaseOnCollisionEnter2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionEnter2D(contact);   // コンポーネントに通知
    };
    void BaseOnCollisionStay2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionStay2D(contact);   // コンポーネントに通知
    };
    void BaseOnCollisionExit2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnCollisionExit2D(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerEnter2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerEnter2D(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerStay2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerStay2D(contact);   // コンポーネントに通知
    };
    void BaseOnTriggerExit2D(HitContact2D& contact) override
    {
        for (auto& component : m_components.GetAll()) component->OnTriggerExit2D(contact);   // コンポーネントに通知
    };

#pragma endregion

public:
    // コンポーネント関連
#pragma region ComponentsFunc

    // コンポーネントを追加する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    T* AddComponent()
    { return m_components.Add<T>(); }

    // コンポーネントを1つ取得する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    T* GetComponent()
    { return m_components.Get<T>(); }

    // コンポーネントをすべて取得する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    std::vector<T*> GetComponents()
    { return m_components.Gets<T>(); }

    // 参照渡し版
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void GetComponents(std::vector<T*>& array)
    { m_components.Gets<T>(array); }

    // コンポーネントがあるか調べる関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    bool HasComponent()
    { return m_components.Has<T>(); }

    // コンポーネントを削除する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void RemoveComponent(T* component)
    { m_components.Remove(component); }

    // 型指定版
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void RemoveComponent()
    { m_components.Remove<T>(); }

    // 指定した型のコンポーネント全てを削除する関数
    template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void RemoveComponents()
    { m_components.Removes<T>(); }

    // コンポーネントを全て削除する関数(Transformは削除不可)
    void RemoveComponents()
    { m_components.AllRemove(); }

    // コンテナを取得する関数
    ComponentContainer& GetComponentContainer() { return m_components; }

#pragma endregion

    // ----- 内部実装 ------- //
private:
    BaseComponent* GetComponentRaw(
        ComponentID id,
        bool isMain) override
    {
        return m_components.Get(id, isMain);
    }
	void GetComponentsRaw(
		ComponentID id,
		bool isMain,
		std::vector<BaseComponent*>& out) override
	{
		m_components.Gets(id, isMain, out);
	}

    bool HasComponentRaw(
        ComponentID id,
        bool isMain) override
    {
        return m_components.Has(id, isMain);
    }
};
