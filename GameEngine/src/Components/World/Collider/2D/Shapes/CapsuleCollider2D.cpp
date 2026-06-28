//====================================================//
// ファイル名  : CapsuleCollider2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/03
//
// 概要        :  カプセルのコライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Collider/2D/Shapes/CapsuleCollider2D.h"

#include "Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void CapsuleCollider2D::UpdateCache() const
{
	Transform* pT = GetTransform();

	// ----- 中心座標の更新 ----- //
	DirectX::SimpleMath::Vector3 localCenter3D = { GetLocalCenterPos().x, GetLocalCenterPos().y, 0 };
	DirectX::SimpleMath::Vector3 center3D = DirectX::SimpleMath::Vector3::Transform(localCenter3D, pT->GetWorldMatrix());

	auto& world2D = WorldSetting2D::Instance();

	DirectX::SimpleMath::Vector2 center = world2D.World3DToLocal2D(center3D);
	SetWorldPosition(center);

	// ----- ラインのベクトルの更新 ----- //
	{
		DirectX::SimpleMath::Vector2 dir;
		float zAngle = GetRotation();

		switch (m_lineDir)
		{
		case AxisType2D::Horizontal:
			dir = { cosf(zAngle), sinf(zAngle) };
			break;
		case AxisType2D::Vertical:
			dir = { -sinf(zAngle), cosf(zAngle) };
			break;
		default:
			break;
		}
		m_cache.dir = dir;
	}

	// ----- 線の長さの更新 ----- //
	{
		float result = m_capsuleHeight - m_radius * 2;
		m_cache.lineLength = (result > 0 ? result : 0);
	}

	// ----- 2点を更新する関数 ----- //
	{
		DirectX::SimpleMath::Vector2 dir = m_cache.dir;
		float length = m_cache.lineLength;

		DirectX::SimpleMath::Vector2 line = dir * length;

		m_cache.points = std::make_pair(center - line / 2, center + line / 2);
	}

	// ----- AABBの更新 ----- //
	{
		std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> points = m_cache.points;

		DirectX::SimpleMath::Vector2 minP = DirectX::SimpleMath::Vector2::Min(points.first, points.second);
		DirectX::SimpleMath::Vector2 maxP = DirectX::SimpleMath::Vector2::Max(points.first, points.second);

		minP -= { m_radius, m_radius };
		maxP += { m_radius, m_radius };

		SetBoundingBox(AABB2D(minP, maxP));
	}

	// フラグのリセット
	ResetDirty();
	SetChanged(true);

	ApplyVersion();
}
