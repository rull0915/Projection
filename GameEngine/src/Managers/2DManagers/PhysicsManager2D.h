//====================================================//
// ファイル名   : PhysicsManager2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要 : 2D版物理挙動管理クラスです
//
// 更新履歴 :
// 2026/04/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/RigidBody/RigidBody2D.h"
#include "Physics/HitContact.h"

#include <vector>
#include <unordered_set>
#include <memory>

//====================================================//
// 前方宣言
//====================================================//

class CollideManager2D;

//====================================================//
// クラス宣言
//====================================================//
class PhysicsManager2D
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 登録予約中のRigidBody
	std::vector<RigidBody2D*> m_addReserves;
	std::unordered_set<RigidBody2D*> m_removeReserves;

	// 登録されているRigidBody2D
	std::vector<RigidBody2D*> m_rigidBodies;

	// 衝突管理クラス
	std::unique_ptr<CollideManager2D> m_collideManager;

	// 衝突情報
	std::vector<HitContact2D> m_contacts;
	std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D> m_contactMap;

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
public:
	PhysicsManager2D();
	~PhysicsManager2D();
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 更新処理
	void Update(float elapsedTime);

	// 登録予約
	void AddRigidBody(RigidBody2D* r) { m_addReserves.push_back(r); }
	void RemoveRigidBody(RigidBody2D* r){ m_removeReserves.insert(r); }

	// 予約の反映
	void ReflectReserves();

	// 予約済みポインタの追加
	void AddReserved()
	{
		for (auto p : m_addReserves)
		{
			m_rigidBodies.push_back(p);
		}

		m_addReserves.clear();
	}

	// 予約済みポインタの削除
	void RemoveReserved()
	{
		// 削除リストが空なら何もしない
		if (m_removeReserves.empty()) return;

		// 削除リストに含まれているかを調べるラムダ式
		auto shouldRemove = [this](RigidBody2D* base)
			{
				return m_removeReserves.contains(base);
			};

		// リストから削除
		std::erase_if(m_addReserves, shouldRemove);
		std::erase_if(m_rigidBodies, shouldRemove);

		m_removeReserves.clear();
	}

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------
	std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& GetHitList() { return m_contactMap; }

	CollideManager2D* GetCollideManager() { return m_collideManager.get(); }

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

private:
	// 衝突後の補正
	void HittedCorrection();

	void PositionCorrection(HitContact2D& contact);
	void VelocityCorrection(HitContact2D& contact);
};
