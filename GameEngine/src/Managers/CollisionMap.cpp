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

//====================================================//
// 関数宣言
//====================================================//

// 衝突関数の登録関数
bool CollisionMap::Register(uint16_t id1, uint16_t id2, CollisionFunc func)
{
	// 合成IDを生成
	uint32_t sumID = ((uint32_t)id1 << 16) | id2;

	// 既にマップにあるかを調べる
	auto it = m_collisionMap.find(sumID);

	// 登録失敗
	if (it != m_collisionMap.end()) return false;

	// 未登録の組み合わせなら追加
	m_collisionMap.insert({ sumID, func });

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

		// 逆のIDを生成
		uint32_t reverseSumID = ((uint32_t)id2 << 16) | id1;

		// マップに追加
		m_collisionMap.insert({ reverseSumID, reverseFunc });
	}

	// 登録成功
	return true;
}

bool CollisionMap::CheckHit(ColliderBase* col1, ColliderBase* col2, HitInfomation* info)
{
	// IDを取得
	uint16_t id1 = col1->GetID();
	uint16_t id2 = col2->GetID();

	// 合成IDを生成
	uint32_t sumID = ((uint32_t)id1 << 16) | id2;

	// マップに登録されているか調べる
	auto func = m_collisionMap.find(sumID);

	// あれば実行
	if (func != m_collisionMap.end())
	{
		return func->second(col1, col2, info);
	}

	return false;
}

// 衝突関数の登録関数
bool CollisionMap2D::Register(unsigned int id1, unsigned int id2, CollisionFunc func)
{
	// 合成IDを生成
	uint32_t sumID = ((uint32_t)id1 << 16) | (uint16_t)id2;

	// 既にマップにあるかを調べる
	auto it = m_collisionMap.find(sumID);

	// 登録失敗
	if (it != m_collisionMap.end()) return false;

	// 未登録の組み合わせなら追加
	m_collisionMap.insert({ sumID, func });

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

		// 逆のIDを生成
		uint32_t reverseSumID = ((uint32_t)id2 << 16) | id1;

		// マップに追加
		m_collisionMap.insert({ reverseSumID, reverseFunc });
	}

	// 登録成功
	return true;
}

bool CollisionMap2D::CheckHit(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info)
{
	// IDを取得
	uint16_t id1 = col1->GetID();
	uint16_t id2 = col2->GetID();

	// 合成IDを生成
	uint32_t sumID = ((uint32_t)id1 << 16) | id2;

	// マップに登録されているか調べる
	auto func = m_collisionMap.find(sumID);

	// あれば実行
	if (func != m_collisionMap.end())
	{
		return func->second(col1, col2, info);
	}

	return false;
}
