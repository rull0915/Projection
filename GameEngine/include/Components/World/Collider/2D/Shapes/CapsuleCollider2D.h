//====================================================//
// ファイル名   : CapsuleCollider2D.h
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
#include "../ColliderBase2D.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

namespace AxisType2D
{
	static constexpr size_t Horizontal = 0;
	static constexpr size_t Vertical = 1;
}

class CapsuleCollider2D : public ColliderBase2D
{
private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 使用する軸 0: x, 1: y
    size_t m_lineDir;

    // ラインの長さ
    float m_capsuleHeight;

    // 半径
    float m_radius;

    // 計算済みワールド情報
    struct WorldCache {
        float lineLength;
        DirectX::SimpleMath::Vector2 dir;
        std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> points;
    };

    mutable WorldCache m_cache;

public:

    //-----------------------------------------------------
    // 生成 / 破棄
    //-----------------------------------------------------
    CapsuleCollider2D(IComponentOwner* own)
        : ColliderBase2D(own)
        , m_lineDir{ 1 }
        , m_capsuleHeight{ 1.0f }
        , m_radius{ 0.5f }
        , m_cache{}
    {
		ADD_PROPATY(m_lineDir);
		ADD_PROPATY(m_capsuleHeight);
		ADD_PROPATY(m_radius);
    };
    ~CapsuleCollider2D() = default;

    void UpdateCache() const override;

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    // ID取得
    unsigned int GetID() override
    {
        return TypeIDGenerator::GetID<CapsuleCollider2D>();
    }

    // ラインの方向ベクトルを返す関数
    DirectX::SimpleMath::Vector2 GetLineDir() const
    {
        if (IsDirty()) UpdateCache();
        return m_cache.dir;
    }

    // 半径を返す関数
    float GetRadius() const
    {
        return m_radius;
    }

    // カプセルの高さを返す関数
    float GetHeight() const
    {
        return m_capsuleHeight;
    }

    // カプセルの軸を返す関数
    size_t GetAxis() const
    {
        return m_lineDir;
    }

    // 線の長さを返す関数
    float GetLineLength() const
    {
        if (IsDirty()) UpdateCache();
        return m_cache.lineLength;
    }
    
    // Lineを構成する2点を返す関数
    std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> GetPoints() const
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

	void SetLineDir(size_t type)
	{
		m_lineDir = type;
		SetDirty();
	}
};
