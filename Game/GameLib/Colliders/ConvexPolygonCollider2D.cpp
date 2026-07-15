//====================================================//
// 
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

#include <algorithm>

#include "Settings/WorldSetting2D.h"
#include "Renderer/Renderer.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

ConvexPolygonCollider2D::ConvexPolygonCollider2D(REngine::IComponentOwner* owner)
	: ColliderBase2D(owner)
	, m_localVertices{}
	, m_worldVertices{}
	, m_worldVertices3D{}
{
}

void ConvexPolygonCollider2D::CorrectionClockWise()
{
	// 凸包でなければ何もしない
	if (m_localVertices.size() < 3) return;

	// 0->1と1->2の外積を調べる
	DirectX::SimpleMath::Vector3 line1 = { m_localVertices[1].x - m_localVertices[0].x, m_localVertices[1].y - m_localVertices[0].y, 0 };
	DirectX::SimpleMath::Vector3 line2 = { m_localVertices[2].x - m_localVertices[1].x, m_localVertices[2].y - m_localVertices[1].y, 0 };

	float cross = line1.Cross(line2).z;

	// 反時計回りなら
	if (cross > 0)
	{
		// 並びを反転
		std::reverse(m_localVertices.begin(), m_localVertices.end());
	}
	
	// Dirtyに
	SetDirty();
}

void ConvexPolygonCollider2D::UpdateCache() const
{
	REngine::Transform* pT = GetTransform();

	// ----- 中心座標の更新 ----- //
	SimpleMath::Vector3 localCenter3D = { GetLocalCenterPos().x, GetLocalCenterPos().y, 0 };
	SimpleMath::Vector3 center3D = SimpleMath::Vector3::Transform(localCenter3D, pT->GetWorldMatrix());

	auto& world2D = REngine::WorldSetting2D::Instance();

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

		REngine::AABB2D boundingBox(min, max);

		SetBoundingBox(boundingBox);
	}

	// フラグのリセット
	ResetDirty();
	SetChanged(true);

	ApplyVersion();
}

void ConvexPolygonCollider2D::DebugRender(REngine::Renderer& renderer, const DirectX::SimpleMath::Color& color)
{
	auto& vertices = GetWorldVertices();
	auto& vertices3D = GetWorldVertices3D();

	for (int i = 0; i < vertices.size(); ++i)
	{
		// 描画
		renderer.Draw().Line(vertices3D[i], vertices3D[(i + 1) % vertices.size()], color);
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

bool ConvexPolygonCollider2D::IsContain(const DirectX::SimpleMath::Vector2& point)
{
	for (auto& vertex : m_localVertices)
	{
		if ((vertex - point).LengthSquared() < FLT_EPSILON)
		{
			return true;
		}
	}

	return false;
}
