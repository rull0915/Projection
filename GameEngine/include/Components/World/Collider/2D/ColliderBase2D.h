//====================================================//
// ファイル名   : ColliderBase2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要 : 2Dコライダー基底クラス
//
// 更新履歴 :
// 2026/04/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderCommon.h"

//====================================================//
// 構造体宣言
//====================================================//
struct AABB2D
{
	DirectX::SimpleMath::Vector2 min;
	DirectX::SimpleMath::Vector2 max;

	AABB2D(DirectX::SimpleMath::Vector2 a, DirectX::SimpleMath::Vector2 b)
		: min{ a }
		, max{ b }
	{
	};

	AABB2D()
		: min{ 0, 0 }, max{ 0, 0 }
	{
	}
};

//====================================================//
// クラス宣言
//====================================================//
class ColliderBase2D : public ColliderCommon
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ローカル中心座標
	DirectX::SimpleMath::Vector2 m_localCenterPos;

	// 回転量
	float m_rotation;

	// 自身を覆うAABB2D
	mutable AABB2D m_boundingBox;

	// ワールド中心座標のキャッシュ
	mutable DirectX::SimpleMath::Vector2 m_worldCenterPos;

public:

	//-----------------------------------------------------
	// 生成 / 破棄
	//-----------------------------------------------------
	ColliderBase2D(IComponentOwner* own)
		: ColliderCommon(own)
		, m_localCenterPos{ 0, 0 }
		, m_rotation{ 0.0f }
		, m_boundingBox{}
		, m_worldCenterPos{}
	{
		ADD_PROPATY(m_localCenterPos);
		ADD_PROPATY(m_rotation);
	}

	virtual ~ColliderBase2D() = default;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// カテゴリを描画に指定
	ComponentCategory GetCategory() const override { return Category::Collider2D; }

	inline AABB2D& GetBoundingBox() const { return m_boundingBox; } // 自身を覆うAABBを取得する関数

	// ワールド座標系での中心座標を返す関数
	DirectX::SimpleMath::Vector2 GetWorldCenterPos() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldCenterPos;
	}
	// ローカル座標系
	DirectX::SimpleMath::Vector2 GetLocalCenterPos() const { return m_localCenterPos; }

	float GetRotation() const { return m_rotation; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	void SetLocalPos(DirectX::SimpleMath::Vector2 pos)
	{
		m_localCenterPos = pos;
		SetDirty();
	}
	void SetRotation(float rot)
	{
		m_rotation = rot;
		SetDirty();
	}

	//-----------------------------------------------------
	// その他関数
	//-----------------------------------------------------

	// キャッシュの更新をする関数
	virtual void UpdateCache() const = 0;

protected:

	inline void SetWorldPosition(const DirectX::SimpleMath::Vector2& pos) const { m_worldCenterPos = pos; }
	inline void SetBoundingBox(const AABB2D& box) const { m_boundingBox = box; }
};
