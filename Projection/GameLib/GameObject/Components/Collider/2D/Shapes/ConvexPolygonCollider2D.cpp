//====================================================//
// ファイル名  : ConvexPolygonCollider2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/03
//
// 概要       : 2次元凸多角形コライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ConvexPolygonCollider2D.h"

#include "GameLib/GameObject/Settings/WorldSetting2D.h"

#include "GameLib/Common/Renderer/Renderer.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

ConvexPolygonCollider2D::ConvexPolygonCollider2D(IComponentOwner* owner)
    : Collider2D(owner, ColliderType2D::ConvexPolygon)
    , m_localVertices{}
    , m_worldVertices{}
    , m_worldVertices3D{}
{
}

void ConvexPolygonCollider2D::UpdateCache() const
{
	Transform* pT = GetTransform();

    // ----- 中心座標の更新 ----- //
    SimpleMath::Vector3 localCenter3D = { GetLocalCenterPos().x, GetLocalCenterPos().y, 0 };
    SimpleMath::Vector3 center3D = SimpleMath::Vector3::Transform(localCenter3D, pT->GetWorldMatrix());

    auto& world2D = WorldSetting2D::Instance();

    SimpleMath::Vector2 center = world2D.World3DToLocal2D(center3D);
    SetWorldPosition(center);

	// ----- 頂点座標の更新 ----- //
	m_worldVertices.clear();
	m_worldVertices3D.clear();

    m_worldVertices.resize(m_localVertices.size());
	m_worldVertices3D.resize(m_localVertices.size());

    for (size_t i = 0; i < m_localVertices.size(); ++i)
    {
        // 中心基準に回転する
		float rotation = GetRotation();

		SimpleMath::Vector2 rotatedVertex =
		{
			m_localVertices[i].x * cosf(rotation) + m_localVertices[i].y * sinf(rotation),
			m_localVertices[i].x * sinf(rotation) - m_localVertices[i].y * cosf(rotation)
		};

        m_worldVertices[i] = center + rotatedVertex;

		m_worldVertices3D[i] = world2D.Local2DToWorld3D(m_worldVertices[i]);
    }

    // AABBの更新
    if (size_t count = m_worldVertices.size(); count > 0)
    {
        SimpleMath::Vector2 min = m_worldVertices[0], max = m_worldVertices[0];

        for (size_t i = 1; i < count; ++i)
        {
            SimpleMath::Vector2 p = m_worldVertices[i];

            if (p.x < min.x) min.x = p.x;
            if (p.y < min.y) min.y = p.y;
            if (p.x > max.x) max.x = p.x;
            if (p.y > max.y) max.y = p.y;
        }

        AABB2D boundingBox(min, max);

        SetBoundingBox(boundingBox);
    }
}

void ConvexPolygonCollider2D::DebugDraw(Renderer & renderer, int color) const
{
    if (IsDirty()) UpdateCache();

    for (int i = 0; i < m_worldVertices.size(); ++i)
    {
        // 描画
		renderer.Draw().Line(m_worldVertices3D[i], m_worldVertices3D[(i + 1) % m_worldVertices.size()], color);
    }
}

bool ConvexPolygonCollider2D::IsConvex() const
{
    if (IsDirty()) UpdateCache();

    const size_t count = m_worldVertices.size();

	// 3点以下は常に凸として扱う
    if (count < 3)
        return true;

    bool hasPositive = false;
    bool hasNegative = false;

     for (size_t i = 0; i < count; i++)
    {
		// 3点を取得
        const SimpleMath::Vector2& prev = m_worldVertices[(i + count - 1) % count];
        const SimpleMath::Vector2& curr = m_worldVertices[i];
        const SimpleMath::Vector2& next = m_worldVertices[(i + 1) % count];

        // 2つの辺を構築
        SimpleMath::Vector2 edge1 =
        {
            curr.x - prev.x,
            curr.y - prev.y
        };
        SimpleMath::Vector2 edge2 =
        {
            next.x - curr.x,
            next.y - curr.y
        };

        // 外積のz成分だけ求める
        float cross =
            edge1.x * edge2.y -
            edge1.y * edge2.x;

        if (cross > 0.0f)
            hasPositive = true;
        else if (cross < 0.0f)
            hasNegative = true;

        // 符号が混在したら凹
        if (hasPositive && hasNegative)
            return false;
    }

    return true;
}
