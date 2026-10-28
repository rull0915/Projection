/**
 * @file    CollisionMap.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 */

#include "pch.h"

#include "Managers/CollisionMap.h"
#include "Components/World/Collider/3D/ColliderBase.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"

namespace RHEngine
{
	// 衝突関数の登録関数
	bool CollisionMap::Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func)
	{
		// 既にマップにあるかを調べる
		auto it = m_collisionMap.find({ id1, id2 });

		// 登録失敗
		if (it != m_collisionMap.end()) return false;

		// 未登録の組み合わせなら追加
		m_collisionMap.insert({ { id1, id2 }, func });

		// IDが異なるの衝突判定関数の場合
		if (id1 != id2)
		{
			// 順序逆の関数を生成
			CollisionFunc reverseFunc = [=](ColliderBase* pCol1, ColliderBase* Col2, HitInfomation* pInfo) -> bool
				{
					// 当たっているかどうか
					bool result = func(Col2, pCol1, pInfo);

					// 衝突情報を反転させる
					if (result && pInfo) *pInfo = pInfo->Inverse();

					// 判定結果を返す
					return result;
				};

			// マップに追加
			m_collisionMap.insert({ { id2, id1 }, reverseFunc});
		}

		// 登録成功
		return true;
	}

	bool CollisionMap::CheckHit(ColliderBase* pCol1, ColliderBase* pCol2, HitInfomation* pInfo)
	{
		// IDを取得
		Component::TypeId id1 = pCol1->TypeId();
		Component::TypeId id2 = pCol2->TypeId();

		// マップに登録されているか調べる
		auto func = m_collisionMap.find({ id1, id2 });

		// あれば実行
		if (func != m_collisionMap.end())
		{
			return func->second(pCol1, pCol2, pInfo);
		}

		return false;
	}

	// 衝突関数の登録関数
	bool CollisionMap2D::Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func)
	{
		// 既にマップにあるかを調べる
		auto it = m_collisionMap.find({ id1, id2 });

		// 登録失敗
		if (it != m_collisionMap.end()) return false;

		// 未登録の組み合わせなら追加
		m_collisionMap.insert({ { id1, id2 }, func });

		// IDが異なる衝突判定関数の場合
		if (id1 != id2)
		{
			// 順序逆の関数を生成
			CollisionFunc reverseFunc = [=](ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info) -> bool
				{
					// 当たっているかどうか
					bool result = func(col2, col1, info);

					// 衝突情報を反転させる
					if (result && info) *info = info->Inverse();

					// 判定結果を返す
					return result;
				};

			// マップに追加
			m_collisionMap.insert({ { id2, id1 }, reverseFunc});
		}

		// 登録成功
		return true;
	}

	bool CollisionMap2D::CheckHit(ColliderBase2D* pCol1, ColliderBase2D* pCol2, HitInfomation2D* pInfo)
	{
		// IDを取得
		Component::TypeId id1 = pCol1->TypeId();
		Component::TypeId id2 = pCol2->TypeId();

		// マップに登録されているか調べる
		auto func = m_collisionMap.find({ id1, id2 });

		// あれば実行
		if (func != m_collisionMap.end())
		{
			return func->second(pCol1, pCol2, pInfo);
		}

		return false;
	}
}	// namespace RHEngine
