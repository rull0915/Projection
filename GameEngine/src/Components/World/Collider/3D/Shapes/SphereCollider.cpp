/**
 * @file    SphereCollider.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/03
 */

#include "pch.h"
#include "Components/World/Collider/3D/Shapes/SphereCollider.h"

#include "Renderer/Renderer.h"

namespace RHEngine
{
	SphereCollider::SphereCollider(IComponentOwner* pOwner)
		: ColliderBase(pOwner)
		, m_radius{ 0.5f }
		, m_worldRadius{ 1.0f }
	{
		ADD_PROPERTY(SphereCollider, m_radius);
	}

	void SphereCollider::UpdateCache() const
	{
		Transform* pTransform = GetTransform();

		// ワールドの拡大率を取得
		DirectX::SimpleMath::Vector3 worldScale = pTransform->GetWorldScale();

		// ----- 中心座標の更新 ----- //
		DirectX::SimpleMath::Vector3 world = DirectX::SimpleMath::Vector3::Transform(GetLocalCenterPos(), pTransform->GetWorldMatrix());
		SetWorldPosition(world);

		// ----- 半径の更新 ----- //
		float max = std::max(std::max(worldScale.x, worldScale.y), worldScale.z);

		m_worldRadius = max * m_radius;

		// ----- BoundingBoxの更新 ----- //
		DirectX::SimpleMath::Vector3 size = { m_worldRadius, m_worldRadius, m_worldRadius };
		SetBoundingBox(BoundingBox(world - size, world + size));

		// フラグのリセット
		ResetDirty();
		SetChanged(true);
		ApplyVersion();
	}

	void SphereCollider::DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color)
	{
		// ワールド行列の算出(Rot,Pos)
		Transform* pTransform = GetTransform();
		DirectX::SimpleMath::Vector3 pos = GetWorldCenterPos();
		DirectX::SimpleMath::Quaternion rot = pTransform->GetWorldRotation();

		// 半径の取得
		float rad = GetRadius();

		// 描画
		renderer.Draw().Primitive().DrawCircle(pos, pTransform->GetUp(), rad, 16, color, false);
		renderer.Draw().Primitive().DrawCircle(pos, pTransform->GetRight(), rad, 16, color, false);
		renderer.Draw().Primitive().DrawCircle(pos, pTransform->GetForward(), rad, 16, color, false);
	}
}	// namespace RHEngine
