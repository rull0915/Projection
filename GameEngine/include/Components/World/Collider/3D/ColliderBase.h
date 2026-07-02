//====================================================//
// ファイル名   : ColliderBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : 衝突判定の基底クラス
//
// 更新履歴 :
// 2026/03/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderCommon.h"

//====================================================//
// 列挙型宣言
//====================================================//
enum class ColliderType
{
	Sphere,
	Line,
	Capsule,
	Box,
};

//====================================================//
// 構造体宣言
//====================================================//
struct AABB
{
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;

	AABB(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b)
		: min{ a }
		, max{ b }
	{
	};

	AABB() 
		: min{ 0, 0, 0 }, max{ 0, 0, 0 }
	{}
};

//====================================================//
// クラス宣言
//====================================================//
class ColliderBase : public ColliderCommon
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	
	// コライダーのタイプ
	const ColliderType m_type;

	// ローカル中心座標
	DirectX::SimpleMath::Vector3 m_localCenterPos;

	// 自身を覆うAABB
	mutable AABB m_boundingBox;

	// ワールド中心座標のキャッシュ
	mutable DirectX::SimpleMath::Vector3 m_worldCenterPos;

public:

	//-----------------------------------------------------
	// 生成 / 破棄
	//-----------------------------------------------------
	ColliderBase(IComponentOwner* own, ColliderType type)
		: ColliderCommon(own)
		, m_type{ type }
		, m_boundingBox{ {0, 0, 0}, {0, 0, 0} }
		, m_localCenterPos{ 0, 0, 0 }
	{
		ADD_PROPERTY(m_localCenterPos);
	}

	virtual ~ColliderBase() = default;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// カテゴリを3Dコライダーに指定
	ComponentCategory GetCategory() const override { return Category::Collider; }

	ColliderType GetType() const { return m_type; };

	inline AABB& GetBoundingBox() const{ return m_boundingBox; } // 自身を覆うAABBを取得する関数

	// ワールド座標系での中心座標を返す関数
	DirectX::SimpleMath::Vector3 GetWorldCenterPos() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldCenterPos;
	}
	// ローカル座標系
	DirectX::SimpleMath::Vector3 GetLocalCenterPos() const { return m_localCenterPos; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	void SetLocalPos(DirectX::SimpleMath::Vector3 pos)
	{
		m_localCenterPos = pos;
		SetDirty();
	}

	//-----------------------------------------------------
	// その他関数
	//-----------------------------------------------------
	
	// キャッシュの更新をする関数
	virtual void UpdateCache() const = 0;

protected:

	inline void SetWorldPosition(const DirectX::SimpleMath::Vector3& pos) const { m_worldCenterPos = pos; }
	inline void SetBoundingBox(const AABB& box) const { m_boundingBox = box; }
};
