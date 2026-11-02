/**
 * @file    GameObject.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/19
 *
 * @brief   ゲームオブジェクトクラス
 */

#pragma once

#include <type_traits>
#include <string>
#include <SimpleMath.h>
#include <vector>

#include "Common/UUID.h"
#include "Common/Property/PropertyObject.h"
#include "Interface/IColliderReceiver.h"
#include "Interface/IComponentOwner.h"
#include "ComponentContainer.h"
#include "Components/ComponentTypeId.h"
#include "Components/ComponentBase.h"

namespace RHEngine
{
	class Scene;

	/**
	 * @brief ゲームオブジェクト コンポーネントを保持する
	 * @details コンポーネントを保持し、シーン内のオブジェクトとして管理される。
	 *          生成はObjectFactoryまたはPrefabBuilderに制限される。
	 */
	class GameObject final
		: public PropertyObject			// プロパティ
		, public IComponentOwner        // コンポーネントに公開するインターフェース
		, public IColliderReceiver      // コライダーの衝突応答を受け取るインターフェース
	{
	public:
		/**
		 * @brief ゲームオブジェクトを作成できるクラスを制限するためのトークン
		 * @details 通常のクラスは生成できない用privateでコンストラクタを宣言
		 *			作成可能とするクラスをfriend宣言
		 */
		class CreateToken
		{
		private:
			CreateToken() = default;

			friend class ObjectFactory;
			friend class PrefabBuilder;
		};

	private:
		/** @brief コンポーネントのコンテナ */
		ComponentContainer m_components;

		/** @brief 識別用UUID */
		UUID m_uuid;

		/** @brief 所属するシーンのポインタ */
		Scene* m_pScene;

		/** @brief アクティブフラグが変更されたかどうか */
		bool m_changedActive;

		/** @brief 親がアクティブかどうか */
		bool m_parentIsActive;

		/** @brief アクティブフラグ */
		bool m_isActive;

		/** @brief 削除フラグ */
		bool m_isDead;

		/**
		 * @brief シリアライズするかどうか
		 * @details オフにすると保存もGUIへの表示もされない
		 */
		bool m_isSerializable;

		/** @brief タグ名 */
		std::string m_tag;

		/** @brief オブジェクト名 */
		std::string m_name;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param  生成可能なクラスを宣言するトークン
		 */
		explicit GameObject(CreateToken);

		~GameObject() = default;

		/// @}

		/**
		 * @brief 終了時にシーンに通知し、コンポーネントを削除する
		 */
		void Finalize();

		/**
		 * @brief GUI変更時処理
		 */
		void OnValidate();

		/**
		 * @brief アクティブ状況の変更を調べ関数を呼び出す
		 */
		void ActiveReserve();

		/**
		 * @brief オブジェクトを生成する
		 * @param position 生成座標
		 * @return 生成したオブジェクト
		 */
		GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });

		/**
		 * @brief 親オブジェクトを取得する
		 */
		GameObject* GetParent() const;

		/**
		 * @brief 子供の数を取得する
		 */
		size_t GetChildCount() const;

		/**
		 * @brief 子供を全て取得する
		 */
		std::vector<GameObject*> GetChildren() const;

		/** @brief 自身を削除する */
		void Destroy() override;

		/// @name 衝突コールバック
		/// @{

		void OnCollisionEnter(HitContact& contact) override;	///< 3Dで衝突した瞬間に呼ばれる
		void OnCollisionStay(HitContact& contact) override;		///< 3Dで衝突している間常に呼ばれる
		void OnCollisionExit(HitContact& contact) override;		///< 3Dで衝突が終わった時に呼ばれる
		void OnTriggerEnter(HitContact& contact) override;		///< 3Dでトリガー領域に侵入した瞬間に呼ばれる
		void OnTriggerStay(HitContact& contact) override;		///< 3Dでトリガー領域に入っている間常に呼ばれる
		void OnTriggerExit(HitContact& contact) override;		///< 3Dでトリガー領域から出たときに呼ばれる

		void OnCollisionEnter2D(HitContact2D& contact) override;	///< 2Dで衝突した瞬間に呼ばれる
		void OnCollisionStay2D(HitContact2D& contact) override;		///< 2Dで衝突している間常に呼ばれる
		void OnCollisionExit2D(HitContact2D& contact) override;		///< 2Dで衝突が終わった時に呼ばれる
		void OnTriggerEnter2D(HitContact2D& contact) override;		///< 2Dでトリガー領域に侵入した瞬間に呼ばれる
		void OnTriggerStay2D(HitContact2D& contact) override;		///< 2Dでトリガー領域に入っている間常に呼ばれる
		void OnTriggerExit2D(HitContact2D& contact) override;		///< 2Dでトリガー領域から出たときに呼ばれる

		/// @}

		/// @name コンポーネント操作
		/// @{

		/**
		 * @brief コンポーネントを追加する
		 * @tparam T 追加するコンポーネントの型
		 * @return 追加したコンポーネント
		 * @details 最も古いコンポーネントが取得されます
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* AddComponent()
		{
			return m_components.Add<T>();
		}

		/**
		 * @brief コンポーネントを取得する
		 * @tparam T 取得するコンポーネントの型
		 * @return 取得したコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* GetComponent() const
		{
			return m_components.Get<T>();
		}

		/**
		 * @brief 指定したコンポーネントを全て取得する
		 * @tparam T 取得するコンポーネントの型
		 * @return 取得したいコンポーネントリスト
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& GetComponents() const
		{
			return m_components.Gets<T>();
		}

		/**
		 * @brief 全てのコンポーネントを取得する
		 * @return コンポーネントリスト
		 */
		const std::vector<ComponentBase*>& GetAllComponents() const
		{
			return m_components.Gets<ComponentBase>();
		}

		/**
		 * @brief コンポーネントを削除する
		 * @tparam T 削除するコンポーネントの型
		 * @param component 削除するコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponent(T* component)
		{
			m_components.Remove(component);
		}

		/**
		 * @brief コンポーネントを削除する
		 * @tparam T 削除するコンポーネントの型
		 * @details 最も古いコンポーネントが削除されます
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponent()
		{
			m_components.Remove<T>();
		}

		/**
		 * @brief 指定したコンポーネントを全て削除する
		 * @tparam T 削除するコンポーネントの型
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void RemoveComponents()
		{
			m_components.Removes<T>();
		}

		/// @}

		/// @name ゲッター
		/// @{

		/** @brief UUID */
		UUID GetUUID() const { return m_uuid; }

		/** @brief アクティブフラグ */
		bool IsActive() const override { return m_isActive && m_parentIsActive; }

		/** @brief 死亡フラグ */
		bool IsDead() const { return m_isDead; }

		/** @brief 所属しているシーン */
		Scene* GetScene() const { return m_pScene; }

		/** @brief シリアライズ可能かどうか */
		bool IsSerializable() const { return m_isSerializable; }

		/** @brief タグ名 */
		const std::string& GetTag() const override { return m_tag; }

		/** @brief オブジェクト名 */
		const std::string& GetName() const override { return m_name; }

		/** @brief コンポーネントコンテナ */
		ComponentContainer& GetComponentContainer() { return m_components; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief UUID */
		void SetUUID(UUID uuid) { m_uuid = uuid; }

		/** @brief 親のアクティブ状況 */
		void SetParentActive(bool f);

		/** @brief アクティブフラグ */
		void SetActive(bool f);

		/** @brief シリアライズ可能かどうか */
		void SetSerializable(bool f) { m_isSerializable = f; }

		/** @brief 所属しているシーン */
		void SetScene(Scene* scene)
		{
			m_pScene = scene;
			m_components.SetScene(scene);
		}

		/** @brief タグ名 */
		void SetTag(const std::string& tag) override { m_tag = tag; }

		/** @brief オブジェクト名 */
		void SetName(const std::string& name) override { m_name = name; }

		// ----- 内部実装 ------- //
	private:

		/**
		 * @brief アクティブ状況が変わった時の処理
		 * @param f 変更後のアクティブ状況
		 */
		void OnActiveChanged(bool f);

		/**
		 * @brief IComponentOwner経由でコンポーネントを取得するためのラップ関数
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネント
		 */
		ComponentBase* GetComponentRaw(
			Component::TypeId id
		) const override
		{
			return m_components.Get(id);
		}

		/**
		 * @brief IComponentOwner経由でコンポーネントを全て取得するためのラップ関数
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネントリスト
		 */
		const std::vector<ComponentBase*>& GetComponentsRaw(
			Component::TypeId id
		) const override
		{
			return m_components.Gets(id);
		}
	};
}	// namespace RHEngine
