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
// 2026/08/28 メンバにUUIDを追加
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>

#include "Common/UUID.h"
#include "Common/Property/PropertyObject.h"
#include "Interface/IColliderReceiver.h"
#include "Interface/IComponentOwner.h"

#include "ComponentContainer.h"

namespace REngine
{
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

		// UUID
		UUID m_uuid;

		// 自身を持つシーンのポインタ
		Scene* m_pScene;

		// アクティブ変更フラグ
		bool m_changedActive;

		// 親がアクティブかどうかのフラグ
		bool m_parentIsActive;

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

		void Finalize();

		void OnValidate();

		// アクティブ変更の適用
		void Reserve();

		GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });

		// 親を取得する関数
		GameObject* GetParent() const;

		// 子供の数を取得する関数
		size_t GetChildCount() const;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		UUID GetUUID() const { return m_uuid; }

		bool IsActive() const override { return m_isActive && m_parentIsActive; }

		bool IsDead() const { return m_isDead; }

		Scene* GetScene() const { return m_pScene; }

		bool IsInvincible() const { return m_isInvincible; }

		const std::string& GetTag() const override { return m_tag; }

		const std::string& GetName() const override { return m_name; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetUUID(UUID uuid) { m_uuid = uuid; }

		void SetParentActive(bool f);

		void SetActive(bool f);

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
			for (auto& component : GetAllComponents()) component->OnCollisionEnter(contact);   // コンポーネントに通知
		};
		void BaseOnCollisionStay(HitContact& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnCollisionStay(contact);   // コンポーネントに通知
		};
		void BaseOnCollisionExit(HitContact& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnCollisionExit(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerEnter(HitContact& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerEnter(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerStay(HitContact& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerStay(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerExit(HitContact& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerExit(contact);   // コンポーネントに通知
		};

		// 2D
		void BaseOnCollisionEnter2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnCollisionEnter2D(contact);   // コンポーネントに通知
		};
		void BaseOnCollisionStay2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnCollisionStay2D(contact);   // コンポーネントに通知
		};
		void BaseOnCollisionExit2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnCollisionExit2D(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerEnter2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerEnter2D(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerStay2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerStay2D(contact);   // コンポーネントに通知
		};
		void BaseOnTriggerExit2D(HitContact2D& contact) override
		{
			for (auto& component : GetAllComponents()) component->OnTriggerExit2D(contact);   // コンポーネントに通知
		};

#pragma endregion

	public:
		// コンポーネント関連
#pragma region ComponentsFunc

		// コンポーネントを追加する関数
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* AddComponent()
		{
			return m_components.Add<T>();
		}

		// コンポーネントを1つ取得する関数
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* GetComponent() const
		{
			return m_components.Get<T>();
		}

		// 指定したコンポーネントをすべて取得する関数
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& GetComponents() const
		{
			return m_components.Gets<T>();
		}

		// コンポーネントをすべて取得する関数
		const std::vector<ComponentBase*>& GetAllComponents() const
		{
			return m_components.Gets<ComponentBase>();
		}

		// コンポーネントを削除する関数
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponent(T* component)
		{
			m_components.Remove(component);
		}

		// 型指定版
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponent()
		{
			m_components.Remove<T>();
		}

		// 指定した型のコンポーネント全てを削除する関数
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponents()
		{
			m_components.Removes<T>();
		}

		// コンテナを取得する関数
		ComponentContainer& GetComponentContainer() { return m_components; }

	private:
		// コンポーネントを全て削除する関数
		void RemoveComponents()
		{
			m_components.Removes<ComponentBase>();
		}

#pragma endregion

		// ----- 内部実装 ------- //
	private:

		// アクティブ状況が変更されたときに呼ばれる関数
		void OnActiveChanged(bool f);

		// コンポーネントを取得するラップ関数
		ComponentBase* GetComponentRaw(
			Component::TypeId id
		) const override
		{
			return m_components.Get(id);
		}

		// 全てのコンポーネントを取得するラップ関数
		const std::vector<ComponentBase*>& GetComponentsRaw(
			Component::TypeId id
		) const override
		{
			return m_components.Gets(id);
		}
	};
} // namespace REngine
