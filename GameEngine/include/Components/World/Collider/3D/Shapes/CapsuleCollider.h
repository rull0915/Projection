//====================================================//
// ファイル名   : CapsuleCollider.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : カプセルのコライダー
//
// 更新履歴 :
// 2026/03/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
enum class AxisType
{
	X, Y, Z,
};

class CapsuleCollider : public ColliderBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 使用する軸
	AxisType m_lineDir; // 正規化して使用

	// ラインの長さ
	float m_capsuleHeight;

	// 半径
	float m_radius;

	// 計算済みワールド情報
	struct WorldCache {
		float radius;
		float height;
		float lineLength;
		DirectX::SimpleMath::Vector3 dir;
		std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> points;
	};

	mutable WorldCache m_cache;

public:

	//-----------------------------------------------------
	// 生成 / 破棄
	//-----------------------------------------------------
	CapsuleCollider(IComponentOwner* own)
		: ColliderBase(own, ColliderType::Capsule)
		, m_lineDir{ AxisType::Y }
		, m_capsuleHeight{ 2.0f }
		, m_radius{ 0.5f }
		, m_cache{}
	{
	};
	~CapsuleCollider() = default;

	void UpdateCache() const override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<CapsuleCollider>();
	}

	// ラインの方向ベクトルを返す関数
	DirectX::SimpleMath::Vector3 GetLineDir() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.dir;
	}

	// ラインの基準軸を返す関数
	AxisType GetLineAxis() const
	{
		return m_lineDir;
	}

	// 半径を返す関数
	float GetRadius() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.radius;
	}

	// カプセルの高さを返す関数
	float GetHeight() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.height;
	}

	// 線の長さを返す関数
	float GetLineLength() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.lineLength;
	}
	
	// Lineを構成する2点を返す関数
	std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> GetPoints() const
	{
		if (IsDirty()) UpdateCache();
		return m_cache.points;
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	void SetHeight(float height) 
	{
		m_capsuleHeight = height;
		SetDirty();
	}

	void SetRadius(float radius) 
	{
		m_radius = radius;
		SetDirty();
	}
};