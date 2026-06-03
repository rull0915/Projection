//====================================================//
// ファイル名   : ConvexPolygonCollider2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要 : 2次元凸多角形コライダー
//
// 更新履歴 :
// 2026/06/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <SimpleMath.h>

#include "../Collider2D.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ConvexPolygonCollider2D : public Collider2D<ConvexPolygonCollider2D, ComponentID::ConvexPolygonCollider2D>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 頂点座標のリスト(中心からの相対座標)
	std::vector<DirectX::SimpleMath::Vector2> m_localVertices;

	// ワールド座標系での頂点座標のキャッシュ
	mutable std::vector<DirectX::SimpleMath::Vector2> m_worldVertices;

	// 3D空間での頂点座標のキャッシュ
	mutable std::vector<DirectX::SimpleMath::Vector3> m_worldVertices3D;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ConvexPolygonCollider2D(IComponentOwner* owner);
    ~ConvexPolygonCollider2D() {};

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------
    
	const std::vector<DirectX::SimpleMath::Vector2>& GetWorldVertices() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldVertices;
	}
    
    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

    // 頂点を追加する関数
    void AddVertex(const DirectX::SimpleMath::Vector2& vertex)
    {
        m_localVertices.push_back(vertex);

        // もし追加されることで凸でなくなる場合削除する
        if (!IsConvex())
        {
            m_localVertices.pop_back();
        }

        SetDirty();
    }
    void PopVertex()
    {
        m_localVertices.pop_back();
        SetDirty();
    }
    void ResetVertices()
    {
        m_localVertices.clear();
        SetDirty();
    }
	void SetVertices(const std::vector<DirectX::SimpleMath::Vector2>& vertices)
	{
		m_localVertices = vertices;

        // 指定された配列が凸でない場合
        if (!IsConvex())
        {
            // リセットする
            ResetVertices();
        }

		SetDirty();
	}

    // その他関数
    void UpdateCache() const override;
    void DebugDraw(Renderer& renderer, int color) const override;

    // 凸法であるか調べる関数
	bool IsConvex() const;
};