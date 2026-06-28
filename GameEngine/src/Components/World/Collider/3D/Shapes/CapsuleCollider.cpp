//====================================================//
// ファイル名  : CapsuleCollider.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/03
//
// 概要        :  カプセルのコライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Collider/3D/Shapes/CapsuleCollider.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void CapsuleCollider::UpdateCache() const
{
	Transform* pT = GetTransform();

	// ワールドの拡大率を取得
	DirectX::SimpleMath::Vector3 worldScale = pT->GetWorldScale();

	// ----- 中心座標の更新 ----- //
	DirectX::SimpleMath::Vector3 world = DirectX::SimpleMath::Vector3::Transform(GetLocalCenterPos(), pT->GetWorldMatrix());
	SetWorldPosition(world);

	// ----- ラインのベクトルの更新 ----- //
	{
		DirectX::SimpleMath::Vector3 dir;

		switch (m_lineDir)
		{
		case AxisType::X:
			dir = pT->GetRight();
			break;
		case AxisType::Y:
			dir = pT->GetUp();
			break;
		case AxisType::Z:
			dir = pT->GetForward();
			break;
		default:
			break;
		}

		m_cache.dir = dir;
	}

	// ----- 半径の更新 ----- //
	{
		float ratio = 1;

		switch (m_lineDir)
		{
		case AxisType::X:
			ratio = std::max(worldScale.y, worldScale.z); break;
		case AxisType::Y:
			ratio = std::max(worldScale.x, worldScale.z); break;
		case AxisType::Z:
			ratio = std::max(worldScale.x, worldScale.y); break;
		}

		m_cache.radius = ratio * m_radius;
	}

	// ----- カプセルの高さの更新 ----- //
	{
		float height = 0;

		switch (m_lineDir)
		{
		case AxisType::X:
			height = worldScale.x * m_capsuleHeight;
			break;
		case AxisType::Y:
			height = worldScale.y * m_capsuleHeight;
			break;
		case AxisType::Z:
			height = worldScale.z * m_capsuleHeight;
			break;
		default:
			break;
		}

		m_cache.height = height;
	}

	// ----- 線の長さの更新 ----- //
	{
		float result = m_cache.height - m_cache.radius * 2;
		m_cache.lineLength = (result > 0 ? result : 0);
	}

	// ----- 2点を更新する関数 ----- //
	{
		DirectX::SimpleMath::Vector3 dir = m_cache.dir;
		float length = m_cache.lineLength;

		DirectX::SimpleMath::Vector3 line = dir * length;

		m_cache.points = std::make_pair(world - line / 2, world + line / 2);
	}

	// ----- AABBの更新 ----- //
	{
		std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> points = m_cache.points;
		float r = m_cache.radius;

		DirectX::SimpleMath::Vector3 minP = DirectX::SimpleMath::Vector3::Min(points.first, points.second);
		DirectX::SimpleMath::Vector3 maxP = DirectX::SimpleMath::Vector3::Max(points.first, points.second);

		minP -= { r, r, r };
		maxP += { r, r, r };

		SetBoundingBox(AABB(minP, maxP));
	}

	// フラグのリセット
	ResetDirty();
	SetChanged(true);

	ApplyVersion();
}
