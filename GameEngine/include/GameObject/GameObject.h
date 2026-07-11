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
// 2026/06/02 必要なインターフェースを継承する設計に変更
// 2026/06/24 コンストラクタに引数を追加し特定のクラスからしか実体化できないように変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>

#include "Common/PropertyObject.h"
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

class GameObject final 
	: public PropertyObject			// プロパティ
	, public IComponentOwner        // コンポーネントに公開するインターフェース
	, public IColliderReceiver      // コライダーの衝突応答を受け取るインターフェース
{
public:
	// GameObjectを作成できるクラスを制限する為の仲介クラス
	class CreateToken
	{
	private:
		CreateToken() = default;
		
		// 作成可能クラスをfriendに指定
		friend class ObjectFactory;
		friend class Canvas;
	};
	
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

	// 削除不可フラグ
	bool m_isInvincible;

public:
	// タグ
	std::string m_tag;

	// 名前
	std::string m_name;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	GameObject(CreateToken);

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void BaseFinalize();

	GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------
	bool IsActive() const override { return m_isActive; }
	
	bool IsDead() const { return m_isDead; }

	Scene* GetScene() const { return m_pScene; }

	bool IsInvincible() const { return m_isInvincible; }

	const std::string& GetTag() const override { return m_tag; }

	const std::string& GetName() const override { return m_name; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	void SetActive(bool value) { m_isActive = value; }

	void SetInvincible(bool f) { m_isInvincible = f; }

	void SetScene(Scene* scene) 
	{
		m_pScene = scene; 
		m_components.SetScene(scene);
	}

	void SetTag(const std::string& tag) override { m_tag = tag; }

	void SetName(const std::string& name) override { m_name = name; }

	void Destroy() override { m_isDead = true; }

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
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	T* AddComponent()
	{ return m_components.Add<T>(); }

	// コンポーネントを1つ取得する関数
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	T* GetComponent() const
	{ return m_components.Get<T>(); }

	// カテゴリからコンポーネントを1つ取得する関数
	ComponentBase* GetComponentWithCategory(ComponentCategory category) const
	{ return m_components.GetWithCategory(category); }

	// 指定したコンポーネントをすべて取得する関数
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	std::vector<T*> GetComponents() const
	{ return m_components.Gets<T>(); }

	// カテゴリからコンポーネントを1つ取得する関数
	std::vector<ComponentBase*> GetsComponentWithCategory(ComponentCategory category) const
	{ return m_components.GetsWithCategory(category); }

	// 参照渡し版
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void GetComponents(std::vector<T*>& array) const
	{ m_components.Gets<T>(array); }

	// カテゴリからコンポーネントを1つ取得する関数
	std::vector<ComponentBase*> GetsComponentWithCategory(ComponentCategory category, std::vector<ComponentBase*>& array) const
	{ m_components.GetsWithCategory(category, array); }

	// コンポーネントをすべて取得する関数
	const std::vector<ComponentBase*> GetAllComponents() const
	{ return m_components.GetAll(); }

	// コンポーネントがあるか調べる関数
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	bool HasComponent() const
	{ return m_components.Has<T>(); }

	// コンポーネントを削除する関数
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void RemoveComponent(T* component)
	{ m_components.Remove(component); }

	// 型指定版
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void RemoveComponent()
	{ m_components.Remove<T>(); }

	// カテゴリ指定版
	void RemoveComponentWithCategory(ComponentCategory caterogy)
	{ m_components.RemoveWithCategory(caterogy); }

	// 指定した型のコンポーネント全てを削除する関数
	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void RemoveComponents()
	{ m_components.Removes<T>(); }

	// コンポーネントを全て削除する関数(Transformは削除不可)
	void RemoveComponents()
	{ m_components.AllRemove(); }

	// カテゴリ指定版
	void RemoveComponentsWithCategory(ComponentCategory caterogy)
	{ m_components.RemovesWithCategory(caterogy); }

	// コンテナを取得する関数
	ComponentContainer& GetComponentContainer() { return m_components; }

#pragma endregion

	// ----- 内部実装 ------- //
private:

	// コンポーネントを取得するラップ関数
	ComponentBase* GetComponentRaw(
		unsigned int id
		) override
	{
		return m_components.Get(id);
	}

	// コンポーネントを取得するラップ関数
	ComponentBase* GetComponentWithCategoryRaw(
		ComponentCategory category
		) override
	{
		return m_components.GetWithCategory(category);
	}

	// 全てのコンポーネントを取得するラップ関数
	void GetComponentsRaw(
		unsigned int id,
		std::vector<ComponentBase*>& out) override
	{
		m_components.Gets(id, out);
	}

	// 全てのコンポーネントを取得するラップ関数
	void GetComponentsWithCategoryRaw(
		ComponentCategory category,
		std::vector<ComponentBase*>& out
		) override
	{
		m_components.GetsWithCategory(category, out);
	}

	// コンポーネントがあるかを調べるラップ関数
	bool HasComponentRaw(
		unsigned int id
		) override
	{
		return m_components.Has(id);
	}
};
