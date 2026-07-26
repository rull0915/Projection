//====================================================//
// ファイル名   : ComponentBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要 : 全てのコンポーネントの基底となるクラス
//
// 更新履歴 :
// 2026/04/02 新規作成
// 2026/06/25 IDからカテゴリに管理方法を変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameObject/Interface/IComponentOwner.h"
#include "ComponentCategory.h"

#include "Common/TypeIdGenerator.h"
#include "Common/Property/PropertyObject.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	struct HitContact;
	struct HitContact2D;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ComponentBase : public PropertyObject
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 自身の所有者のポインタ
		IComponentOwner* m_own;

		// アクティブ状況変更フラグ
		bool m_changedActive;

		// アクティブフラグ
		bool m_isActive;

		// 所有者がアクティブかどうか
		bool m_ownerIsActive;

		// Start関数が呼ばれたか
		bool m_isStarted;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ComponentBase(IComponentOwner* own);
		virtual ~ComponentBase();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// 所有者
		IComponentOwner* GetOwn() const { return m_own; }

		// アクティブフラグ
		bool IsActive() const { return m_isActive && m_ownerIsActive; }

		// スタート済みかどうか
		bool IsStarted() const { return m_isStarted; }

		// カテゴリ
		virtual ComponentCategory GetCategory() const { return Category::Original; }

		// ID
		virtual unsigned int GetID() = 0;

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetOwnerActive(bool f);
		void SetActive(bool f);

		void SetStart() { m_isStarted = true; }

	public:

		// アクティブフラグの変更を適応する関数
		void Reserve();

		//-----------------------------------------------------
		// 仮想関数
		//-----------------------------------------------------

		virtual void Awake() {}    // 作成直後
		virtual void Start() {}    // 初回更新の直前

		virtual void Update(const GameTimer&) {}  // 更新関数
		virtual void LateUpdate(const GameTimer&) {}  // 遅延更新関数

		virtual void OnDestroy() {} // 削除時
		virtual void OnValidate() {} // GUIでの値変更時

		virtual void OnEnable() {} // Activeに変更時
		virtual void OnDisable() {} // 非Activeに変更時

		// ----- 衝突関連 ----- //

		// 3D
		// 衝突
		virtual void OnCollisionEnter(HitContact&) {};
		virtual void OnCollisionStay(HitContact&) {};
		virtual void OnCollisionExit(HitContact&) {};
		// トリガー
		virtual void OnTriggerEnter(HitContact&) {};
		virtual void OnTriggerStay(HitContact&) {};
		virtual void OnTriggerExit(HitContact&) {};

		// 2D
		// 衝突
		virtual void OnCollisionEnter2D(HitContact2D&) {};
		virtual void OnCollisionStay2D(HitContact2D&) {};
		virtual void OnCollisionExit2D(HitContact2D&) {};
		// トリガー
		virtual void OnTriggerEnter2D(HitContact2D&) {};
		virtual void OnTriggerStay2D(HitContact2D&) {};
		virtual void OnTriggerExit2D(HitContact2D&) {};

		// ----- ラップ関数 ----- //
		template<typename T>
		T* GetComponent() { return m_own->GetComponent<T>(); }

		template<typename T>
		void GetComponents(std::vector<T*>& vec) { m_own->GetComponents<T>(vec); }

	private:
		// アクティブ状況変化時に呼ばれる関数
		void OnActiveChanged(bool f);
	};

} // namespace REngine
