//====================================================//
// ファイル名   : BoxCollider2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : 2Dボックスコライダー
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderBase2D.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class BoxCollider2D : public ColliderBase2D
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ボックスのローカルでのサイズ
	DirectX::SimpleMath::Vector2 m_localSize;

	// 計算済みワールド情報
	struct WorldCache {
		DirectX::SimpleMath::Vector2 xAxis;
		DirectX::SimpleMath::Vector2 yAxis;
	};

	mutable WorldCache m_cache;


public:

	//-----------------------------------------------------
	// 生成 / 破棄
	//-----------------------------------------------------
	BoxCollider2D(IComponentOwner* own)
		: ColliderBase2D(own)
		, m_localSize{ 1, 1 }
		, m_cache{}
	{
	};
	~BoxCollider2D() = default;

	void UpdateCache() const override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<BoxCollider2D>();
	}

	// 各軸ベクトルを取得する関数
	float GetAngle() const
	{
		return GetRotation();
	}
	DirectX::SimpleMath::Vector2 GetXAxis() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.xAxis;
	}
	DirectX::SimpleMath::Vector2 GetYAxis() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.yAxis;
	}
	DirectX::SimpleMath::Vector2 GetSize() const
	{
		return m_localSize;
	}
	DirectX::SimpleMath::Vector2 GetHalfSize() const
	{
		return m_localSize * 0.5f;
	}

	DirectX::SimpleMath::Vector2 GetLocalSize() const
	{
		return m_localSize;
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetLocalSize(DirectX::SimpleMath::Vector2 scale)
	{
		m_localSize = scale;
		SetDirty();
	}
};