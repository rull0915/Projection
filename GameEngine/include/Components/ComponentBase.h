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

#include "System/TypeIdGenerator.h"
#include "Common/PropertyObject.h"

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
	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 自身の所有者のポインタ
	IComponentOwner* m_own;

	// アクティブフラグ
	bool m_isActive;

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
	IComponentOwner* GetOwn() const { return m_own; }   // 所有者

	bool IsActive() const { return m_isActive; }        // アクティブフラグ

	bool IsStarted() const { return m_isStarted; }      // スタート済みかどうか

	// 所属空間
	virtual ComponentSpace GetSpace() const = 0;

	// カテゴリ
	virtual ComponentCategory GetCategory() const { return Category::Original; }

	// ID
	virtual unsigned int GetID() = 0;

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetActive(bool f) { m_isActive = f; }

	void SetStart() { m_isStarted = true; }

public:

	//-----------------------------------------------------
	// 仮想関数
	//-----------------------------------------------------

	virtual void Awake() {}    // 作成直後
	virtual void Start() {}    // 初回更新の直前

	virtual void Update([[maybe_unused]] const GameTimer& gameTimer) {}  // 更新関数
	virtual void LateUpdate([[maybe_unused]] const GameTimer& gameTimer) {}  // 遅延更新関数

	virtual void OnDestroy() {} // 削除時

	// ----- 衝突関連 ----- //

	// 3D
	// 衝突
	virtual void OnCollisionEnter([[maybe_unused]] HitContact& other) {};
	virtual void OnCollisionStay([[maybe_unused]] HitContact& other) {};
	virtual void OnCollisionExit([[maybe_unused]] HitContact& other) {};
	// トリガー
	virtual void OnTriggerEnter([[maybe_unused]] HitContact& other) {};
	virtual void OnTriggerStay([[maybe_unused]] HitContact& other) {};
	virtual void OnTriggerExit([[maybe_unused]] HitContact& other) {};

	// 2D
	// 衝突
	virtual void OnCollisionEnter2D([[maybe_unused]] HitContact2D& other) {};
	virtual void OnCollisionStay2D([[maybe_unused]] HitContact2D& other) {};
	virtual void OnCollisionExit2D([[maybe_unused]] HitContact2D& other) {};
	// トリガー
	virtual void OnTriggerEnter2D([[maybe_unused]] HitContact2D& other) {};
	virtual void OnTriggerStay2D([[maybe_unused]] HitContact2D& other) {};
	virtual void OnTriggerExit2D([[maybe_unused]] HitContact2D& other) {};
	
	// ----- ラップ関数 ----- //
	template<typename T>
	T* GetComponent() { return m_own->GetComponent<T>(); }

	template<typename T>
	void GetComponents(std::vector<T*>& vec) { m_own->GetComponents<T>(vec); }
};
