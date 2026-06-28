//====================================================//
// ファイル名  : DebugRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DebugRenderer.h"

#include "ConvexPolygonCollider2D.h"
#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace DrawCollider2D
{
	void DrawCollider2D::DrawCollider_Convex(ColliderBase2D* collider, Renderer& renderer, int color)
	{
		ConvexPolygonCollider2D* convex = static_cast<ConvexPolygonCollider2D*>(collider);

		auto& vertices = convex->GetWorldVertices();
		auto& vertices3D = convex->GetWorldVertices3D();

		for (int i = 0; i < vertices.size(); ++i)
		{
			// 描画
			renderer.Draw().Line(vertices3D[i], vertices3D[(i + 1) % vertices.size()], color);
		}
	}
}
