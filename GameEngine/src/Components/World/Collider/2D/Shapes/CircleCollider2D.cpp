//====================================================//
// ファイル名  : CircleCollider2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/18
//
// 概要        : 2Dの円状のコライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Collider/2D/Shapes/CircleCollider2D.h"

#include "Settings/WorldSetting2D.h"
#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void CircleCollider2D::UpdateCache() const
{
	Transform* pT = GetTransform();

	// ----- 中心座標の更新 ----- //
	DirectX::SimpleMath::Vector3 localCenter3D = { GetLocalCenterPos().x, GetLocalCenterPos().y, 0 };
	DirectX::SimpleMath::Vector3 center3D = DirectX::SimpleMath::Vector3::Transform(localCenter3D, pT->GetWorldMatrix());
	
	auto& world2D = WorldSetting2D::Instance();

	DirectX::SimpleMath::Vector2 center = world2D.World3DToLocal2D(center3D);
	SetWorldPosition(center);

	// ----- AABBの更新 ----- //
	DirectX::SimpleMath::Vector3 size = { m_radius, m_radius, m_radius };
	SetBoundingBox(AABB2D(center - size, center + size));

	// フラグのリセット
	ResetDirty();
	SetChanged(true);

	ApplyVersion();
}

void CircleCollider2D::DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color)
{
	// ワールド行列の算出(Rot,Pos)
	DirectX::SimpleMath::Vector2 pos = GetWorldCenterPos();

	auto& world2D = WorldSetting2D::Instance();

	float rad = GetRadius();
	renderer.Draw().Circle(world2D.Local2DToWorld3D(pos), world2D.GetNormal(), rad, 16, color, false);
}
