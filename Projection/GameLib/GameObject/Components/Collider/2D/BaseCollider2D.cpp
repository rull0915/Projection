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

//====================================================//
// 関数の実体宣言
//====================================================//

BaseCollider2D::BaseCollider2D(IComponentOwner* own, ColliderType2D type, ComponentID ID, bool isMain)
	: ICollider(own, ID, isMain)
	, m_type{ type }
	, m_localCenterPos{ 0, 0 }
	, m_boundingBox{}
	, m_worldCenterPos{}
{
}

void AABB2D::DebugDraw(Renderer& renderer, int color) const
{
	renderer.Draw().Line({ min.x, min.y, 0 }, { max.x, min.y, 0 }, color);
	renderer.Draw().Line({ min.x, max.y, 0 }, { max.x, max.y, 0 }, color);
	renderer.Draw().Line({ min.x, min.y, 0 }, { min.x, max.y, 0 }, color);
	renderer.Draw().Line({ max.x, min.y, 0 }, { max.x, max.y, 0 }, color);
}
