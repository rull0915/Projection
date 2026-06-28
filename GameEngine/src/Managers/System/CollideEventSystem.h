//====================================================//
// ファイル名   : CollideEventSystem.h
// 作成者       : 
// 作成日       : 2026/04/26
//
// 概要 : 衝突系の関数呼び出しを担当するクラス
//
// 更新履歴 :
// 2026/04/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Physics/HitContact.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class CollideEventSystem
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	std::unordered_map<ObjectPair, HitContact, ObjectPairHash> m_prevMap;
	std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D> m_prevMap2D;

public:
	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	CollideEventSystem()
		: m_prevMap{} 
		, m_prevMap2D{}
	{};
	~CollideEventSystem() {};

public:
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void ResetEvent()
	{
		m_prevMap.clear();
	}

	// コライダーを削除する関数
	void RemoveCollider(ColliderBase* collider);
	void RemoveCollider2D(ColliderBase2D* collider);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------
	void CallCollideFunctions(std::unordered_map<ObjectPair, HitContact, ObjectPairHash>& nowMap);
	void CallCollideFunctions2D(std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& nowMap);

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------
	void OnEnter(HitContact& contact);
	void OnStay(HitContact& contact);
	void OnExit(HitContact& contact);

	void OnEnter2D(HitContact2D& contact);
	void OnStay2D(HitContact2D& contact);
	void OnExit2D(HitContact2D& contact);
};