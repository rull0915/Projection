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
#include "CollideManager.h"

#include "GameLib/GameObject/GameObject.h"

#include "GameLib/GameObject/Settings/PhysicsSettings.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

void CollideManager::AddReserved()
{
	if (m_reserves.size() <= 0) return;

	for (auto& collide : m_reserves)
	{
		m_colliders.push_back(collide);

		// ツリー登録用オブジェクト生成
		ObjectForTree* pOFT = new ObjectForTree();
		pOFT->m_pObject = collide;
		m_treeObjects.push_back(pOFT);

		// ツリーへ登録
		m_tree.AddObject(pOFT);
	}

	m_reserves.clear();
}

void CollideManager::RemoveReserved()
{
	if (m_removeReserves.size() <= 0) return;

	for (auto& collide : m_removeReserves)
	{
		// 8分木から削除
		m_treeObjects.erase(
			std::remove_if(m_treeObjects.begin(), m_treeObjects.end(),
				[collide](const auto& oft) {
					if (oft->m_pObject == collide)
					{
						oft->Remove();
						return true;
					}
					return false;
				}),
			m_treeObjects.end()
		);

		m_colliders.erase(
			std::remove(m_colliders.begin(), m_colliders.end(), collide),
			m_colliders.end()
		);
	}

	m_removeReserves.clear();
}

void CollideManager::UpdateCaches()
{
	// 全てのコライダー
	for (auto& collide : m_colliders)
	{
		// Dirthフラグが立っていればキャッシュを更新
		if(collide->IsDirty()) collide->UpdateCache();
	}
}

void CollideManager::MoveAllColliderOnTree()
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
void CollideManager::CheckHitAll(std::vector<HitContact>& contacts)
{
	// 予約済みコライダーの登録
	AddReserved();

	// 削除予約済みコライダーの削除
	RemoveReserved();

	// 条件のラムダ式
	auto condition = [&](BaseCollider* a, BaseCollider* b)
		{
			// コライダーのアクティブ確認
			if (!a->m_isActive || !b->m_isActive) return false;

			// コライダーの所有者が同じならスキップ
			if (a->m_own == b->m_own) return false;

			// コライダーが持つAABBが衝突していなければスキップ
			AABB aabb1 = a->m_boundingBox, aabb2 = b->m_boundingBox;

			if (aabb1.min.x > aabb2.max.x || aabb2.min.x > aabb1.max.x) return false;
			if (aabb1.min.y > aabb2.max.y || aabb2.min.y > aabb1.max.y) return false;
			if (aabb1.min.z > aabb2.max.z || aabb2.min.z > aabb1.max.z) return false;

			// レイヤー確認
			if (!PhysicsSettings::Instance().CanHit(a->GetLayer(), b->GetLayer())) return false;

			return true;
		};

	// 衝突判定を行うラムダ式
	auto action = [&](BaseCollider* a, BaseCollider* b)
		{
			HitContact contact;

			bool hit = CheckHitPair(a, b, contact);

			if (hit)
			{
				contacts.push_back(contact);
			}
		};

	// 衝突リストの初期化
	contacts.clear();

	// 衝突判定
	m_tree.CheckAllCollisionList(condition, action);
}

bool CollideManager::CheckHitPair(BaseCollider* colA, BaseCollider* colB, HitContact& contact)
{
	// リジッドボディの有無をチェック
	RigidBody* aRigid = colA->GetOwn()->GetComponent<RigidBody>();
	RigidBody* bRigid = colB->GetOwn()->GetComponent<RigidBody>();

	// 非アクティブの場合nullとして扱う
	if (aRigid && !aRigid->IsActive()) aRigid = nullptr;
	if (bRigid && !bRigid->IsActive()) bRigid = nullptr;

	// どちらも物理挙動を持っていなければ判定スキップ
	if (!aRigid && !bRigid) return false;

	bool aSleep = (!aRigid || aRigid->IsSleep());
	bool bSleep = (!bRigid || bRigid->IsSleep());

	// どちらもスリープであれば判定スキップ
	if (aSleep && bSleep) return false;

	// 衝突情報を取得する変数を用意
	HitInfomation hit;

	// トリガーかどうか
	bool hasTrigger = colA->IsTrigger() || colB->IsTrigger();
	// 詳細情報を取得するかどうか
	bool needInfo = colA->IsNeedInfo() || colB->IsNeedInfo();

	// 各オブジェクトが動かないかどうか
	bool AisStatic = !aRigid || (aRigid && aRigid->IsStatic());
	bool BisStatic = !bRigid || (bRigid && bRigid->IsStatic());

	// 両方staticなら判定スキップ
	if (AisStatic&& BisStatic) return false;

	// 衝突判定
	if (CheckHit(colA, colB, (!hasTrigger || needInfo) ? &hit : nullptr))
	{
		contact.own = colA->GetOwn(), contact.other = colB->GetOwn();

		contact.ownCol = colA, contact.otherCol = colB;

		contact.ownRigid = aRigid, contact.otherRigid = bRigid;

		contact.ownIsStatic = AisStatic, contact.otherIsStatic = BisStatic;

		contact.isTrigger = hasTrigger;

		if (!hasTrigger || needInfo)
		{
			contact.normal = hit.hitDir;
			contact.penetration = hit.hitLen;
		}

		return true;
	}

	return false;
}

/// <summary>
/// Rayと登録済みコライダーの衝突判定を行う関数
/// </summary>
bool CollideManager::RayCast(const Ray& ray, float maxDistance, RaycastHit& hit, uint64_t layerMask)
{
	// 衝突情報を初期化
	hit.collider = nullptr;
	hit.distance = FLT_MAX;
	hit.normal = DirectX::SimpleMath::Vector3::Zero;
	hit.point = DirectX::SimpleMath::Vector3::Zero;

	// 情報の取得
	DirectX::SimpleMath::Vector3 min = ray.GetMin(maxDistance), max = ray.GetMax(maxDistance);

	// 全てのコライダーを検索
	for (BaseCollider* collider : m_colliders)
	{
		// レイヤー確認
		int layer = collider->GetLayer();
		if (!(layerMask & (1ULL << layer))) continue;

		// ブロードフェーズ
		AABB aabb = collider->m_boundingBox;

		if (aabb.min.x > max.x || min.x > aabb.max.x) continue;
		if (aabb.min.y > max.y || min.y > aabb.max.y) continue;
		if (aabb.min.z > max.z || min.z > aabb.max.z) continue;

		// ナローフェーズ
		RaycastHit localHit;

		bool isHit = CheckHitWithRay(ray, maxDistance, collider, localHit);

		if (!isHit) continue;

		// 距離最小が更新されたら
		if (localHit.distance < hit.distance)
		{
			// 衝突情報を更新
			hit = localHit;
		}
	}

	// コライダーがnullでなければ少なくとも何かに当たっている
	return hit.collider != nullptr;
}
