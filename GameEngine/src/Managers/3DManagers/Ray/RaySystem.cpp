//====================================================//
// ファイル名  : RaySystem.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RaySystem.h"

#include "Components/World/Collider/3D/ColliderBase.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool RaySystem::Register(uint16_t id, CollisionFunc func)
	{
		// 既にマップにあるかを調べる
		auto it = m_collisionMap.find(id);

		// 登録失敗
		if (it != m_collisionMap.end()) return false;

		// 未登録の組み合わせなら追加
		m_collisionMap.insert({ id, func });

		return true;
	}

	bool RaySystem::CheckHit(const Ray& ray, float max, ColliderBase* collider, RaycastHit* raycastHit)
	{
		// ID取得
		uint16_t id = collider->GetID();

		// マップにあるかを調べる
		auto it = m_collisionMap.find(id);

		// 存在していれば
		if (it != m_collisionMap.end())
		{
			// 実行
			return it->second(ray, max, collider, raycastHit);
		}

		return false;
	}

	bool RaySystem::RayCast(const std::vector<ColliderBase*>& colliders, const Ray& ray, float maxDistance, RaycastHit* hit, uint64_t layerMask)
	{
		// 衝突情報を初期化
		hit->collider = nullptr;
		hit->distance = FLT_MAX;
		hit->normal = DirectX::SimpleMath::Vector3::Zero;
		hit->point = DirectX::SimpleMath::Vector3::Zero;

		// 情報の取得
		DirectX::SimpleMath::Vector3 min = ray.GetMin(maxDistance), max = ray.GetMax(maxDistance);

		// 全てのコライダーを検索
		for (ColliderBase* collider : colliders)
		{
			if (collider->IsTrigger()) continue;

			// レイヤー確認
			int layer = collider->GetLayer();
			if (!(layerMask & (1ULL << layer))) continue;

			// ブロードフェーズ
			AABB aabb = collider->GetBoundingBox();

			if (aabb.min.x > max.x || min.x > aabb.max.x) continue;
			if (aabb.min.y > max.y || min.y > aabb.max.y) continue;
			if (aabb.min.z > max.z || min.z > aabb.max.z) continue;

			// ナローフェーズ
			RaycastHit localHit;

			bool isHit = CheckHit(ray, maxDistance, collider, &localHit);

			if (!isHit) continue;

			// 距離最小が更新されたら
			if (localHit.distance < hit->distance)
			{
				// 衝突情報を更新
				*hit = localHit;
			}
		}

		// コライダーがnullでなければ少なくとも何かに当たっている
		return hit->collider != nullptr;
	}
}	// namespace REngine
