//====================================================//
// ファイル名  : BoxCollider2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要        :  2Dボックスのコライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "BoxCollider2D.h"

#include "GameLib/Common/Renderer/Renderer.h"

#include "GameLib/GameObject/Settings/WorldSetting2D.h"

#include <array>

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

void BoxCollider2D::UpdateCache() const
{
    Transform* pT = GetTransform();
    
    // ワールドの拡大率を取得
    SimpleMath::Vector3 worldScale = pT->GetWorldScale();

    // ----- 中心座標の更新 ----- //
    DirectX::SimpleMath::Vector3 localCenter3D = { GetLocalCenterPos().x, GetLocalCenterPos().y, 0 };
    DirectX::SimpleMath::Vector3 center3D = DirectX::SimpleMath::Vector3::Transform(localCenter3D, pT->GetWorldMatrix());

    auto& world2D = WorldSetting2D::Instance();

    DirectX::SimpleMath::Vector2 center = world2D.World3DToLocal2D(center3D);
    SetWorldPosition(center);


    // ----- 各軸の更新 ----- //
    float zAngle = GetRotation();
    m_cache.xAxis = { cosf(zAngle), sinf(zAngle) };
    m_cache.yAxis = { -sinf(zAngle), cosf(zAngle) };

    // ----- AABBの更新 ----- //
    SimpleMath::Vector2 h = m_localSize * 0.5f;

    float ex = abs(m_cache.xAxis.x * h.x) + abs(m_cache.yAxis.x * h.y);
    float ey = abs(m_cache.xAxis.y * h.x) + abs(m_cache.yAxis.y * h.y);

    SimpleMath::Vector2 extent(ex, ey);

    // extentからmin maxを計算
    SetBoundingBox(AABB2D(center - extent, center + extent));

    // フラグのリセット
    ResetDirty();
    SetChanged(true);

    ApplyVersion();
}

void BoxCollider2D::DebugDraw(Renderer& renderer, int color) const
{
    // ワールド行列の算出(Rot,Pos)
    SimpleMath::Vector2 pos = GetWorldCenterPos();

    SimpleMath::Vector2 halfSize = GetHalfSize();
    SimpleMath::Vector2 xA = GetXAxis();
    SimpleMath::Vector2 yA = GetYAxis();

    // PointList
    std::array<SimpleMath::Vector2, 4> points =
    {
        pos + (halfSize.x * xA + halfSize.y * yA),
        pos + (halfSize.x * xA + -halfSize.y * yA),
        pos + (-halfSize.x * xA + -halfSize.y * yA),
        pos + (-halfSize.x * xA + halfSize.y * yA),
    };

    auto& world2D = WorldSetting2D::Instance();

    DirectX::SimpleMath::Vector3 right = world2D.GetXAxis(), up = world2D.GetYAxis();

    std::array<SimpleMath::Vector3, 4> worldPoints =
    {
        right * points[0].x + up * points[0].y,
        right * points[1].x + up * points[1].y,
        right * points[2].x + up * points[2].y,
        right * points[3].x + up * points[3].y
    };

    for (int i = 0; i < 4; i++)
    {
        renderer.Draw().Line(
            DirectX::SimpleMath::Vector3{ worldPoints[i] },
            DirectX::SimpleMath::Vector3{ worldPoints[(i + 1) % 4] }, 
            color);
    }
}
