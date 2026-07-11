//====================================================//
// ファイル名  : CheckHit.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CheckHit.h"

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "../GameEngine/src/Managers/2DManagers/Collider/CheckHit/CheckHitAuxiliary2D.h"
#include "ConvexPolygonCollider2D.h"
#include "CollisionAuxiliary.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace Collision2D
{
	using namespace DirectX;

	bool CheckHit2D_Convex_Convex(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info)
	{
		// コライダーの変換
		ConvexPolygonCollider2D* colA = static_cast<ConvexPolygonCollider2D*>(col1);
		ConvexPolygonCollider2D* colB = static_cast<ConvexPolygonCollider2D*>(col2);

		// 各ポリゴンの頂点情報
		const std::vector<SimpleMath::Vector2>& aVertices = colA->GetWorldVertices(), bVertices = colB->GetWorldVertices();

		// どちらかがポリゴンでなかった場合リターン
		if (aVertices.size() < 3 || bVertices.size() < 3) return false;

		// 調べる軸を用意
		std::vector<SimpleMath::Vector2> axes;

		// Aの辺の法線
		for (size_t i = 0; i < aVertices.size(); ++i)
		{
			// 2点間のベクトルを作成
			SimpleMath::Vector2 edge = aVertices[(i + 1) % aVertices.size()] - aVertices[i];

			// 2点が同じ座標にあれば追加しない
			if (edge.LengthSquared() < FLT_EPSILON)
			{
				continue;
			}

			// 法線を求める
			SimpleMath::Vector2 normal = { -edge.y, edge.x };

			// 正規化し追加する
			normal.Normalize();
			axes.push_back(normal);
		}

		// Bの辺の法線
		for (size_t i = 0; i < bVertices.size(); ++i)
		{
			// 2点間のベクトルを作成
			SimpleMath::Vector2 edge = bVertices[(i + 1) % bVertices.size()] - bVertices[i];

			// 2点が同じ座標にあれば追加しない
			if (edge.LengthSquared() < FLT_EPSILON)
			{
				continue;
			}

			// 法線を求める
			SimpleMath::Vector2 normal = { -edge.y, edge.x };

			// 正規化し追加する
			normal.Normalize();
			axes.push_back(normal);
		}

		// 投影情報を格納する変数
		CollisionAuxiliary::Projection aProj{}, bProj{};

		// MTVを調べる変数
		DirectX::SimpleMath::Vector2 mtvAxis;
		float minOverLap = FLT_MAX;

		// 各軸に対して分離しているか確認
		for (const auto& axis : axes)
		{
			// 2つの多角形を軸に投影
			aProj = CollisionAuxiliary::Project(aVertices, axis);
			bProj = CollisionAuxiliary::Project(bVertices, axis);

			// 重なり量を調べる
			float overLap = GetOverlap(aProj, bProj);

			// 離れていれば早期リターン
			if (overLap <= 0) return false;

			// MTVを更新したら
			if (overLap < minOverLap)
			{
				minOverLap = overLap;
				mtvAxis = axis;
			}
		}

		// 全ての軸で衝突していれば

		// 衝突情報が必要なら
		if (info)
		{
			DirectX::SimpleMath::Vector2 interval = colA->GetWorldCenterPos() - colB->GetWorldCenterPos();

			if (mtvAxis.Dot(interval) > 0.0f)
			{
				mtvAxis *= -1.0f;
			}
			info->own = colA->GetOwn();
			info->target = colB->GetOwn();

			info->hitDir = mtvAxis;
			info->hitLen = minOverLap;
		}

		return true;
	}

	bool CheckHit2D_Circle_Convex(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info)
	{
		// コライダーを変換
		CircleCollider2D* colA = static_cast<CircleCollider2D*>(col1);
		ConvexPolygonCollider2D* colB = static_cast<ConvexPolygonCollider2D*>(col2);

		// 全ての辺に対して最短距離を求めることで衝突を調べる。

		// 調べる線分を用意
		std::vector<std::pair<SimpleMath::Vector2, SimpleMath::Vector2>> edges;

		const std::vector<SimpleMath::Vector2>& vertices = colB->GetWorldVertices();

		// 線分を生成
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			// 2点を繋ぐ
			edges.push_back({ vertices[i], vertices[(i + 1) % vertices.size()] });
		}

		// 円の情報を取得
		SimpleMath::Vector2 center = colA->GetWorldCenterPos();
		float radius = colA->GetRadius();

		// 最大最小点をキャッシュしておく
		SimpleMath::Vector2 minPoint{};
		float minDistanceSq = FLT_MAX;

		// 円がポリゴン内部にあるかを調べるための変数
		bool hasNegative = false, hasPositive = false;

		// 生成した線分を調べる
		for (auto& edge : edges)
		{
			// 線分と点の最近点を求める
			SimpleMath::Vector2 closedPoint = ClosestPointOnSegment(edge.first, edge.second, center);

			float lenSq = (closedPoint - center).LengthSquared();

			// 点が辺のどちら側にあるか調べる
			SimpleMath::Vector2 toPoint = center - edge.first, line = edge.second - edge.first;
			float cross = toPoint.x * line.y - toPoint.y * line.x;

			if (cross > 0) hasPositive = true;
			if (cross < 0) hasNegative = true;

			// 最小を更新したら
			if (lenSq < minDistanceSq)
			{
				minDistanceSq = lenSq;
				minPoint = closedPoint;
			}
		}

		// 円中心がポリゴン内部の場合
		if (!hasNegative || !hasPositive)
		{
			// 衝突情報が必要なら
			if (info)
			{
				info->own = colA->GetOwn();
				info->target = colB->GetOwn();

				SimpleMath::Vector2 dir = center - minPoint;
				float len = dir.Length();

				info->hitDir = dir / len;
				info->hitLen = radius + len;
			}
		}

		// 円中心がポリゴン外部の場合
		else
		{
			// 最も近い点が半径より離れていればfalse
			if (minDistanceSq > radius * radius) return false;

			// ぶつかっていれば

			// 衝突情報が必要なら
			if (info)
			{
				info->own = colA->GetOwn();
				info->target = colB->GetOwn();

				SimpleMath::Vector2 dir = minPoint - center;
				float len = dir.Length();

				info->hitDir = dir / len;
				info->hitLen = radius - len;
			}
		}

		return true;
	}

	bool CheckHit2D_Capsule_Convex(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info)
	{
		// コライダーを変換
		CapsuleCollider2D* colA = static_cast<CapsuleCollider2D*>(col1);
		ConvexPolygonCollider2D* colB = static_cast<ConvexPolygonCollider2D*>(col2);

		// ポリゴンの頂点を取得
		const std::vector<SimpleMath::Vector2>& vertices = colB->GetWorldVertices();

		// 調べる分離軸を作成
		std::vector<SimpleMath::Vector2> axes;

		// ポリゴンの法線
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			// 2点間のベクトルを作成
			SimpleMath::Vector2 edge = vertices[(i + 1) % vertices.size()] - vertices[i];

			// 2点が同じ座標にあれば追加しない
			if (edge.LengthSquared() < FLT_EPSILON)
			{
				continue;
			}

			// 法線を求める
			SimpleMath::Vector2 normal = { -edge.y, edge.x };

			// 正規化し追加する
			normal.Normalize();
			axes.push_back(normal);
		}

		// カプセルの2点からポリゴンへの最短ベクトル
		std::vector<SimpleMath::Vector2> capsule{ colA->GetPoints().first, colA->GetPoints().second };

		// カプセルの垂線
		{
			SimpleMath::Vector2 line = capsule[1] - capsule[0];
			SimpleMath::Vector2 normal = { -line.y, line.x };
			normal.Normalize();
			axes.push_back(normal);
		}

		// 最も近い点のインデックスを取得
		size_t startNear = CollisionAuxiliary::GetNearestPoint(vertices, capsule[0]), 
				endNear = CollisionAuxiliary::GetNearestPoint(vertices, capsule[1]);

		// 正規化し配列に追加
		{
			SimpleMath::Vector2 axis = capsule[0] - vertices[startNear];
			axis.Normalize();
			axes.push_back(axis);
		}
		{
			SimpleMath::Vector2 axis = capsule[1] - vertices[endNear];
			axis.Normalize();
			axes.push_back(axis);
		}

		CollisionAuxiliary::Projection capsuleProjection{}, polygonProjection;

		SimpleMath::Vector2 mtvAxis{};
		float minOverLap = FLT_MAX;

		float radius = colA->GetRadius();

		// 分離軸候補を調べる
		for (const auto& axis : axes)
		{
			capsuleProjection = CollisionAuxiliary::Project(capsule, axis);
			polygonProjection = CollisionAuxiliary::Project(vertices, axis);

			// 重なり量を調べる
			float overLap = CollisionAuxiliary::GetOverlap(capsuleProjection, polygonProjection) + radius;

			// 離れていれば早期リターン
			if (overLap <= 0) return false;

			// MTVを更新したら
			if (overLap < minOverLap)
			{
				minOverLap = overLap;
				mtvAxis = axis;
			}
		}

		// 全ての軸で衝突していれば

		// 衝突情報が必要なら
		if (info)
		{
			DirectX::SimpleMath::Vector2 interval = colA->GetWorldCenterPos() - colB->GetWorldCenterPos();

			if (mtvAxis.Dot(interval) > 0.0f)
			{
				mtvAxis *= -1.0f;
			}
			info->own = colA->GetOwn();
			info->target = colB->GetOwn();

			info->hitDir = mtvAxis;
			info->hitLen = minOverLap;
		}

		return true;
	}
}
