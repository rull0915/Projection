//====================================================//
// ファイル名  : BaseCollider2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要        :  2Dコライダー基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "BaseCollider2D.h"

#include "GameLib/Common/Renderer/Renderer.h"

#include "GameLib/GameObject/Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

BaseCollider2D::BaseCollider2D(IComponentOwner* own, ColliderType2D type, ComponentID ID, bool isMain)
	: ICollider(own, ID, isMain)
	, m_type{ type }
	, m_localCenterPos{ 0, 0 }
	, m_rotation{ 0.0f }
	, m_boundingBox{}
	, m_worldCenterPos{}
{
}

void AABB2D::DebugDraw(Renderer& renderer, int color) const
{
	// 点を作成
	DirectX::SimpleMath::Vector3 points[4] =
	{
		WorldSetting2D::Instance().Local2DToWorld3D({ min.x, min.y }),
		WorldSetting2D::Instance().Local2DToWorld3D({ min.x, max.y }),
		WorldSetting2D::Instance().Local2DToWorld3D({ max.x, max.y }),
		WorldSetting2D::Instance().Local2DToWorld3D({ max.x, min.y })
	};

	// 描画
	for (size_t i = 0; i < 4; ++i)
	{
		renderer.Draw().Line(points[i], points[(i + 1) % 4], color);
	}
}
