//====================================================//
// ファイル名  : CollisionMap.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要        : 衝突関数マップ
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#include "Managers/CollisionMap.h"
#include "Components/World/Collider/3D/ColliderBase.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"

namespace REngine
{
	//====================================================//
	// 関数宣言
	//====================================================//

	// 衝突関数の登録関数
	bool CollisionMap::Register(ComponentBase::TypeId id1, ComponentBase::TypeId id2, CollisionFunc func)
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
			CollisionFunc reverseFunc = [=](ColliderBase* col1, ColliderBase* col2, HitInfomation* info) -> bool
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

	bool CollisionMap::CheckHit(ColliderBase* col1, ColliderBase* col2, HitInfomation* info)
	{
		// IDを取得
		ComponentBase::TypeId id1 = col1->StaticTypeId();
		ComponentBase::TypeId id2 = col2->StaticTypeId();

		// マップに登録されているか調べる
		auto func = m_collisionMap.find({ id1, id2 });

		// あれば実行
		if (func != m_collisionMap.end())
		{
			return func->second(col1, col2, info);
		}

		return false;
	}

	// 衝突関数の登録関数
	bool CollisionMap2D::Register(ComponentBase::TypeId id1, ComponentBase::TypeId id2, CollisionFunc func)
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

	bool CollisionMap2D::CheckHit(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info)
	{
		// IDを取得
		ComponentBase::TypeId id1 = col1->StaticTypeId();
		ComponentBase::TypeId id2 = col2->StaticTypeId();

		// マップに登録されているか調べる
		auto func = m_collisionMap.find({ id1, id2 });

		// あれば実行
		if (func != m_collisionMap.end())
		{
			return func->second(col1, col2, info);
		}

		return false;
	}
}	// namespace REngine
