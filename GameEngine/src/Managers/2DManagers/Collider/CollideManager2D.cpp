//====================================================//
// ファイル名  : SampleClass.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要        :  衝突判定管理クラスです
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CollideManager2D.h"

#include "Managers/CollisionMap.h"
#include "Settings/WorldSetting2D.h"

#include "Physics/HitInfomation2D.h"

#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "Components/World/RigidBody/RigidBody2D.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

void CollideManager2D::AddReserved()
{
	for (auto& collide : m_addReserves)
	{
		m_colliders.push_back(collide);

		// ツリー登録用オブジェクト生成
		ObjectForTree2D* pOFT = new ObjectForTree2D();
		pOFT->m_pObject = collide;
		m_treeObjects.push_back(pOFT);

		// ツリーへ登録
		m_tree.AddObject(pOFT);
	}

	m_addReserves.clear();
}

void CollideManager2D::RemoveReserved()
{
	// 削除リストが空なら何もしない
	if (m_removeReserves.empty()) return;

	// 8分木から削除
	std::erase_if(m_treeObjects,
		[this](const auto& oft)
		{
			if (m_removeReserves.contains(oft->m_pObject))
			{
				oft->Remove();
				return true;
			}
			return false;
		});

	// 削除リストに含まれているかを調べるラムダ式
	auto shouldRemove = [this](ColliderBase2D* base)
		{
			return m_removeReserves.contains(base);
		};

	// リストから削除
	std::erase_if(m_addReserves, shouldRemove);
	std::erase_if(m_colliders, shouldRemove);

	m_removeReserves.clear();
}

void CollideManager2D::UpdateCaches()
{
	// 全てのコライダー
	for (auto& collide : m_colliders)
	{
		// Dirthフラグが立っていればキャッシュを更新
		if(collide->IsDirty()) collide->UpdateCache();
	}
}

void CollideManager2D::MoveAllColliderOnTree()
{
	// 予約済みコライダーの登録
	AddReserved();

	// 削除予約済みコライダーの削除
	RemoveReserved();

	// 全オブジェクトのチェック
	for (size_t i = 0; i < m_colliders.size(); i++)
	{
		// もし値が変わっていたら
		if (m_colliders[i]->IsDirty())
		{
			// キャッシュの更新
			m_colliders[i]->UpdateCache();
		}
		// もし変更フラグが立っていたら
		if (m_colliders[i]->IsChanged())
		{
			// 木構造を更新

			// 一度リストから外れる
			m_treeObjects[i]->Remove();

			// 再登録
			m_tree.AddObject(m_treeObjects[i]);

			m_colliders[i]->ResetChangeFrag();
		}
	}
}

/// <summary>
/// 全てのコライダーの衝突判定を行う関数
/// </summary>
void CollideManager2D::CheckHitAll(std::vector<HitContact2D>& contacts)
{
	// 予約済みコライダーの登録
	AddReserved();

	// 削除予約済みコライダーの削除
	RemoveReserved();

	// 条件のラムダ式
	auto condition = [&](ColliderBase2D* a, ColliderBase2D* b)
		{
			// コライダーのアクティブ確認
			if (!a->IsActive() || !b->IsActive()) return false;

			// コライダーの所有者が同じならスキップ
			if (a->GetOwn() == b->GetOwn()) return false;

			// コライダーが持つAABBが衝突していなければスキップ
			AABB2D aabb1 = a->GetBoundingBox(), aabb2 = b->GetBoundingBox();

			if (aabb1.min.x > aabb2.max.x || aabb2.min.x > aabb1.max.x) return false;
			if (aabb1.min.y > aabb2.max.y || aabb2.min.y > aabb1.max.y) return false;

			return true;
		};

	// 衝突判定を行うラムダ式
	auto action = [&](ColliderBase2D* a, ColliderBase2D* b)
		{
			HitContact2D contact;

			bool hit = CheckHitPair(a, b, contact);

			if (hit)
			{
				contacts.push_back(contact);
			}
		};

	// 衝突リストの初期化
	contacts.clear();

// 世界の設定が変わっていれば全てのコライダーをDirtyにを更新

	if (WorldSetting2D::Instance().IsDirty())
	{
		// 全てのコライダー
		for (auto& collide : m_colliders)
		{
			// Dirthフラグが立っていればキャッシュを更新
			collide->SetDirty();
		}

		WorldSetting2D::Instance().ResetDirty();
	}


	// 衝突判定
	m_tree.CheckAllCollisionList(condition, action);
}

bool CollideManager2D::CheckHitPair(ColliderBase2D* colA, ColliderBase2D* colB, HitContact2D& contact)
{
	// リジッドボディの有無をチェック
	RigidBody2D* aRigid = colA->GetOwn()->GetComponent<RigidBody2D>();
	RigidBody2D* bRigid = colB->GetOwn()->GetComponent<RigidBody2D>();

	// どちらも持っていなければ判定スキップ
	if (!aRigid && !bRigid) return false;

	bool aSleep = (!aRigid || aRigid->IsSleep());
	bool bSleep = (!bRigid || bRigid->IsSleep());

	// どちらもスリープであれば判定スキップ
	if (aSleep && bSleep) return false;

	// 衝突情報を取得する変数を用意
	HitInfomation2D hit;

	// 詳細情報を取得するかどうか
	bool hasTrigger = colA->IsTrigger() || colB->IsTrigger();

	// 各オブジェクトが動かないかどうか
	bool AisStatic = !aRigid || (aRigid && aRigid->IsStatic());
	bool BisStatic = !bRigid || (bRigid && bRigid->IsStatic());

	// 両方staticなら判定スキップ
	if (AisStatic && BisStatic) return false;

	// 衝突判定
	if (CollisionMap2D::CheckHit(colA, colB, !hasTrigger ? &hit : nullptr))
	{
		contact.own = colA->GetOwn(), contact.other = colB->GetOwn();

		contact.ownCol = colA, contact.otherCol = colB;

		contact.ownRigid = aRigid, contact.otherRigid = bRigid;

		contact.ownIsStatic = AisStatic, contact.otherIsStatic = BisStatic;

		contact.isTrigger = hasTrigger;

		if (!hasTrigger)
		{
			contact.normal = hit.hitDir;
			contact.penetration = hit.hitLen;
		}

		return true;
	}

	return false;
}
