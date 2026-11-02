/**
 * @file    ComponentBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/02
 *
 * @brief   全てのコンポーネントの基底となるクラス
 */

#pragma once

#include <vector>

#include "Common/UUID.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "ComponentTypeId.h"
#include "Common/Property/PropertyObject.h"

namespace RHEngine
{
	struct HitContact;
	struct HitContact2D;
	class GameTimer;

	/**
	 * @brief 全てのコンポーネントの基底となるクラス
	 */
	class ComponentBase : public PropertyObject
	{
	private:

		/** @brief 識別用UUID */
		UUID m_uuid;

		/** @brief 所有者のポインタ */
		IComponentOwner* m_pOwner;

		/** @brief アクティブ状況が変更されたかどうか */
		bool m_changedActive;

		/** @brief アクティブかどうか */
		bool m_isActive;

		/** @brief 所有者がアクティブかどうか */
		bool m_ownerIsActive;

		/** @brief 既にStart関数が呼ばれたかどうか */
		bool m_isStarted;

	public:

		/// @ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		ComponentBase(IComponentOwner* pOwner);

		virtual ~ComponentBase();

		/// @}

	public:

		/**
		 * @brief 追加直後に呼び出される
		 */
		virtual void Awake() {}

		/**
		 * @brief Updateが初めて実行される直前に呼び出される
		 */
		virtual void Start() {}

		/**
		 * @brief 毎フレーム呼び出される
		 */
		virtual void Update(const GameTimer&) {}

		/**
		 * @brief 毎フレームUpdateの後に呼び出される
		 * @param  
		 */
		virtual void LateUpdate(const GameTimer&) {}

		/**
		 * @brief 削除時に呼び出される
		 */
		virtual void OnDestroy() {}

		/**
		 * @brief GUI上で値が変更されたときに呼び出される
		 */
		virtual void OnValidate() {}

		/**
		 * @brief 非アクティブからアクティブへ変更されたときに呼ばれる
		 */
		virtual void OnEnable() {}

		/**
		 * @brief アクティブから非アクティブへ変更されたときに呼ばれる
		 */
		virtual void OnDisable() {}

		/// @name 衝突コールバック
		/// @{

		virtual void OnCollisionEnter(HitContact&) {};	///< 3Dで衝突した瞬間に呼ばれる
		virtual void OnCollisionStay(HitContact&) {};	///< 3Dで衝突している間常にに呼ばれる
		virtual void OnCollisionExit(HitContact&) {};	///< 3Dで衝突が終わった時に呼ばれる

		virtual void OnTriggerEnter(HitContact&) {};	///< 3Dでトリガー領域に侵入した瞬間に呼ばれる
		virtual void OnTriggerStay(HitContact&) {};		///< 3Dでトリガー領域に入っている間常に呼ばれる
		virtual void OnTriggerExit(HitContact&) {};		///< 3Dでトリガー領域から出たときに呼ばれる

		virtual void OnCollisionEnter2D(HitContact2D&) {};	///< 2Dで衝突した瞬間に呼ばれる
		virtual void OnCollisionStay2D(HitContact2D&) {};	///< 2Dで衝突している間常にに呼ばれる
		virtual void OnCollisionExit2D(HitContact2D&) {};	///< 2Dで衝突が終わった時に呼ばれる

		virtual void OnTriggerEnter2D(HitContact2D&) {};	///< 2Dでトリガー領域に侵入した瞬間に呼ばれる
		virtual void OnTriggerStay2D(HitContact2D&) {};		///< 2Dでトリガー領域に入っている間常に呼ばれる
		virtual void OnTriggerExit2D(HitContact2D&) {};		///< 2Dでトリガー領域から出たときに呼ばれる

		/// @}

		/**
		 * @brief コンポーネントを取得する
		 * @tparam T 取得したいコンポーネントの型
		 * @return コンポーネントのポインタ
		 */
		template<typename T>
		T* GetComponent() { return m_pOwner->GetComponent<T>(); }

		/**
		 * @brief 特定の型の全てのコンポーネントを取得する
		 * @tparam T 取得したいコンポーネントの型
		 * @return 取得したコンポーネントリスト
		 */
		template<typename T>
		const std::vector<ComponentBase*>& GetComponents() { m_pOwner->GetComponents<T>(); }

		/// @name タイプシステム
		/// @{

		/**
		 * @brief クラスごとに一意なIDを取得する
		 * @return ID
		 */
		static Component::TypeId StaticTypeId()
		{
			static char id;
			return &id;
		}

		/**
		 * @brief StaticTypeIdを取得する仮想関数
		 * @return ID
		 */
		virtual Component::TypeId TypeId() const
		{
			return StaticTypeId();
		}

		/**
		 * @brief タイプの一致を判別する
		 * @param id 調べるID
		 * @return 一致したかどうか
		 */
		virtual bool IsTypeOf(Component::TypeId id) const
		{
			return id == StaticTypeId();
		}

		/**
		 * @brief 自分が対応するIDを集める
		 * @param out 対応するIDのリスト
		 */
		virtual void CollectTypeIds(std::vector<Component::TypeId>& out) const
		{
			out.push_back(StaticTypeId());
		}

		/// @}

		/**
		 * @brief アクティブフラグの変更を検知し子に通知、関数呼び出しを行う
		 */
		void Reserve();

		/// @name ゲッター
		/// @{

		/** @brief 識別用UUID */
		UUID GetUUID() const { return m_uuid; }

		/** @brief 所有者のポインタ */
		IComponentOwner* GetOwn() const { return m_pOwner; }

		/** @brief アクティブかどうか */
		bool IsActive() const { return m_isActive && m_ownerIsActive; }

		/** @brief 既にStart関数が呼ばれたかどうか */
		bool IsStarted() const { return m_isStarted; }

		/**
		 * @brief 1つのGameObjectに複数付けられるかどうか
		 * @TODO 実装
		 */
		virtual bool AllowMultiple() const { return true; }

		/// @}

		/// @name セッター

		/** @brief 識別用UUID */
		void SetUUID(UUID uuid) { m_uuid = uuid; }

		/** @brief 所有者のアクティブ状況 */
		void SetOwnerActive(bool f);

		/** @brief アクティブかどうか */
		void SetActive(bool f);

		/** @brief スタートが呼ばれた際の処理 */
		void SetStart() { m_isStarted = true; }

	private:
		// アクティブ状況変化時に呼ばれる関数
		void OnActiveChanged(bool f);
	};

	/**
	 * @brief コンポーネントの型判定 (継承関係を含む) に必要な関数群を定義する
	 * @details 各コンポーネントのpublic領域に一度だけ書く必要がある
	 *			次の関数が定義される
	 *			- StaticTypeId	: クラスごとに一意なID (static変数アドレス)
	 *			- TypeId		: StaticTypeIdを返す仮想関数版
	 *			- IsTypeOf      : 自身またはBase以前のいずれかとIDが一致すればtrue
	 *			- CollectTypeIds: 自身から基底クラスまでのIDをoutに集める	
	 *
	 *          IsTypeOf が基底側も辿るため、GetComponent<基底クラス>() で
	 *          派生クラスのコンポーネントも取得できる。
	 * 
	 *          Base には直接の基底クラスを指定すること。Base 自身も
	 *          COMPONENT_TYPE を使っているか、ComponentBase であること。
	 *          書き忘れると、そのクラスは基底クラスとして検索されなくなる。
	 * @param Base 直接の基底クラス
	 */
#define DECLARE_COMPONENT_TYPE(Base)													\
	static RHEngine::Component::TypeId StaticTypeId()									\
	{																					\
		static char id;																	\
		return &id;																		\
	}																					\
																						\
	virtual RHEngine::Component::TypeId TypeId() const override							\
	{																					\
		return StaticTypeId();															\
	}																					\
																						\
	bool IsTypeOf(RHEngine::Component::TypeId id) const override						\
	{																					\
		return id == StaticTypeId() || Base::IsTypeOf(id);								\
	}																					\
																						\
	void CollectTypeIds(std::vector<RHEngine::Component::TypeId>& out) const override	\
	{																					\
		out.push_back(StaticTypeId());													\
		Base::CollectTypeIds(out);														\
	}

} // namespace RHEngine
