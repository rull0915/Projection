//====================================================//
// ファイル名  : CheckHit2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要        : 2D衝突判定の関数をまとめたcpp 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CheckHit2D.h"
#include "GameLib/GameMath/GameMath.h"
#include "CheckHitAuxiliary2D.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

bool CheckHit2D(BaseCollider2D* colA, BaseCollider2D* colB, HitInfomation2D* info)
{
	switch (colA->GetType())
	{
	case ColliderType2D::Circle:
		switch (colB->GetType())
		{
		case ColliderType2D::Circle:
			return CheckHit2D(static_cast<CircleCollider2D*>(colA), static_cast<CircleCollider2D*>(colB), info);
		case ColliderType2D::Box:
			return CheckHit2D(static_cast<CircleCollider2D*>(colA), static_cast<BoxCollider2D*>(colB), info);
		case ColliderType2D::Capsule:
			return CheckHit2D(static_cast<CircleCollider2D*>(colA), static_cast<CapsuleCollider2D*>(colB), info);
		case ColliderType2D::ConvexPolygon:
			return CheckHit2D(static_cast<CircleCollider2D*>(colA), static_cast<ConvexPolygonCollider2D*>(colB), info);
		default:
			break;
		}
		break;
	case ColliderType2D::Box:
		switch (colB->GetType())
		{
		case ColliderType2D::Circle:
		{
			bool hit = CheckHit2D(static_cast<CircleCollider2D*>(colB), static_cast<BoxCollider2D*>(colA), info);
			if(hit && info) *info = info->Inverse();
			return hit;
		}
		case ColliderType2D::Box:
			return CheckHit2D(static_cast<BoxCollider2D*>(colA), static_cast<BoxCollider2D*>(colB), info);
		case ColliderType2D::Capsule:
		{
			bool hit = CheckHit2D(static_cast<CapsuleCollider2D*>(colB), static_cast<BoxCollider2D*>(colA), info);
			if (hit && info) *info = info->Inverse();
			return hit;
		}
		default:
			break;
		}
		break;
	case ColliderType2D::Capsule:

		switch (colB->GetType())
		{
		case ColliderType2D::Circle:
		{
			bool hit = CheckHit2D(static_cast<CapsuleCollider2D*>(colB), static_cast<CircleCollider2D*>(colA), info);
			if (hit && info) *info = info->Inverse();
			return hit;
		}		
		case ColliderType2D::Box:
			return CheckHit2D(static_cast<CapsuleCollider2D*>(colA), static_cast<BoxCollider2D*>(colB), info);
		case ColliderType2D::Capsule:
			return CheckHit2D(static_cast<CapsuleCollider2D*>(colA), static_cast<CapsuleCollider2D*>(colB), info);
		case ColliderType2D::ConvexPolygon:
			return CheckHit2D(static_cast<CapsuleCollider2D*>(colA), static_cast<ConvexPolygonCollider2D*>(colB), info);
		default:
			break;
		}
		break;
	case ColliderType2D::ConvexPolygon:

		switch (colB->GetType())
		{
		case ColliderType2D::ConvexPolygon:
			return CheckHit2D(static_cast<ConvexPolygonCollider2D*>(colA), static_cast<ConvexPolygonCollider2D*>(colB), info);
			
		case ColliderType2D::Circle:
		{
			bool hit = CheckHit2D(static_cast<CircleCollider2D*>(colB), static_cast<ConvexPolygonCollider2D*>(colA), info);
			if (hit && info) *info = info->Inverse();
			return hit;
		}
		case ColliderType2D::Capsule:
		{
			bool hit = CheckHit2D(static_cast<CapsuleCollider2D*>(colB), static_cast<ConvexPolygonCollider2D*>(colA), info);
			if (hit && info) *info = info->Inverse();
			return hit;
		}

		default:
			break;
		}

		break;
	default:
		break;
	}

	return false;
}

bool CheckHit2D(CircleCollider2D* colA, CircleCollider2D* colB, HitInfomation2D* info)
{
	// 情報の取得
	SimpleMath::Vector2 centerA = colA->GetWorldCenterPos(), centerB = colB->GetWorldCenterPos();
	float radiusA = colA->GetRadius(), radiusB = colB->GetRadius();

	float lenSq = (centerB - centerA).LengthSquared();

	// 長さの2乗で比較
	bool hit = lenSq <= (radiusA + radiusB) * (radiusA + radiusB);

	// 衝突情報の計算
	if (hit && info)
	{
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();

		float len = (centerB - centerA).Length();

		// 0除算対策
		if (len > EPSILON)
		{
			info->hitDir = (centerB - centerA) / len;
		}
		else
		{
			info->hitDir = SimpleMath::Vector2(1, 0); // 適当な安全方向
		}

		info->hitLen = (radiusA + radiusB) - len;
	}

	return hit;
}

bool CheckHit2D(CircleCollider2D* colA, CapsuleCollider2D* colB, HitInfomation2D* info)
{
	// 各座標を取得
	SimpleMath::Vector2 center = colA->GetWorldCenterPos();
	std::pair<SimpleMath::Vector2, SimpleMath::Vector2> points = colB->GetPoints();

	// 最近点を求める
	SimpleMath::Vector2 nearPoint = ClosestPointOnSegment(points.first, points.second, center);

	// 距離を算出
	float length = (center - nearPoint).LengthSquared();

	// 半径の合計以下かどうかを返す
	float rad1 = colA->GetRadius(), rad2 = colB->GetRadius();

	bool hit = length <= (rad1 + rad2) * (rad1 + rad2);

	// 衝突時
	if (hit && info)
	{
		// 衝突情報の保存
		SimpleMath::Vector2 diff = nearPoint - center;
		float dist = diff.Length();

		if (dist > 0.0001f) {
			info->hitDir = diff / dist;
		}
		else {
			info->hitDir = SimpleMath::Vector2::UnitY; // 重なりすぎている時は真上に逃がす
		}
		info->hitLen = (rad1 + rad2) - dist;
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();
	}

	return hit;
}

bool CheckHit2D(CircleCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info)
{
	// 情報の取得
	SimpleMath::Vector2 posA = colA->GetWorldCenterPos(), posB = colB->GetWorldCenterPos();
	float radius = colA->GetRadius();
	SimpleMath::Vector2 xAxis = colB->GetXAxis(), yAxis = colB->GetYAxis();
	SimpleMath::Vector2 halfSize = colB->GetHalfSize();

	// ボックスから見た円への相対ベクトル
	SimpleMath::Vector2 diff = posA - posB;

	// 球をボックスのローカル座標に変換
	SimpleMath::Vector2 localCenter = {
			diff.Dot(xAxis), diff.Dot(yAxis)
	};

	// ローカル座標系での最近点を求める
	SimpleMath::Vector2 localNear =
	{
		MyMath::Clamp(localCenter.x, -halfSize.x, halfSize.x),
		MyMath::Clamp(localCenter.y, -halfSize.y, halfSize.y),
	};

	// 半径と比較
	bool hit = (localCenter - localNear).LengthSquared() <= (radius * radius);

	// 衝突情報の計算
	if (hit && info)
	{
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();

		// 最近点のワールド座標を求める
		SimpleMath::Vector2 worldNear = posB + (xAxis * localNear.x) + (yAxis * localNear.y);
		
		SimpleMath::Vector2 hitVec = (worldNear - posA);

		float len = hitVec.Length();

		if (len > EPSILON)
		{
			info->hitDir = hitVec / len;
		}
		else
		{
			info->hitDir = SimpleMath::Vector2(1, 0); // 適当な安全方向
		}
		info->hitLen = radius - len;
	}

	return hit;
}

bool CheckHit2D(CapsuleCollider2D* colA, CapsuleCollider2D* colB, HitInfomation2D* info)
{
	// 各座標を取得
	std::pair<SimpleMath::Vector2, SimpleMath::Vector2> points1 = colA->GetPoints();
	std::pair<SimpleMath::Vector2, SimpleMath::Vector2> points2 = colB->GetPoints();

	// 最近点を求める
	SimpleMath::Vector2 pP, pQ;
	float l = ClosedSegmentToSegment(points1.first, points1.second, points2.first, points2.second, pP, pQ);

	// 半径の合計以下かどうかを返す
	float rad1 = colA->GetRadius(), rad2 = colB->GetRadius();
	bool hit = l <= (rad1 + rad2) * (rad1 + rad2);

	// 衝突時
	if (hit && info)
	{
		// 衝突情報の保存
		SimpleMath::Vector2 diff = pQ - pP;
		float dist = diff.Length();

		if (dist > 0.0001f) {
			info->hitDir = diff / dist;
		}
		else {
			info->hitDir = SimpleMath::Vector2::UnitY; // 重なりすぎている時は真上に逃がす
		}
		info->hitLen = (rad1 + rad2) - dist;
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();
	}

	return hit;
}

bool CheckHit2D(CapsuleCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info)
{
	// OBBの各軸3本 カプセルの軸とOBBの軸の外積3本 カプセルの両端からOBBへの線 この8本の分離軸に対して離れているか確認

	// 情報の取得
	std::pair<SimpleMath::Vector2, SimpleMath::Vector2> worldPoints = colA->GetPoints();
	float radius = colA->GetRadius();

	SimpleMath::Vector2 size = colB->GetHalfSize();

	// OBBの軸
	SimpleMath::Vector2 naX = colB->GetXAxis(), naY = colB->GetYAxis();
	SimpleMath::Vector2 aX = naX * size.x, aY = naY * size.y;

	// 線分ベクトル
	SimpleMath::Vector2 lineVec = worldPoints.second - worldPoints.first;
	SimpleMath::Vector2 halfLine = lineVec / 2;
	lineVec.Normalize();

	// 中心間のベクトル
	SimpleMath::Vector2 interval = colA->GetWorldCenterPos() - colB->GetWorldCenterPos();

	// 投影後の長さ格納用変数
	float lA = 0, lB = 0, l = 0;

	// 最小の重なりとその軸
	float minOverLap = FLT_MAX;
	SimpleMath::Vector2 minAxis = { 0, 0 };

	// ----- Aの各軸を判定 ----- //

	// X軸
	lA = size.x;
	lB = abs(naX.Dot(halfLine)) + radius;
	l = abs(naX.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = naX;
	}

	// Y軸
	lA = size.y;
	lB = abs(naY.Dot(halfLine)) + radius;
	l = abs(naY.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = naY;
	}

	SimpleMath::Vector2 centerB = colB->GetWorldCenterPos();

	// ボックスから見た円への相対ベクトル
	SimpleMath::Vector2 diffStart = worldPoints.first - centerB;
	SimpleMath::Vector2 diffGoal = worldPoints.second - centerB;

	// 球をボックスのローカル座標に変換
	SimpleMath::Vector2 localCenterStart = { diffStart.Dot(naX), diffStart.Dot(naY) };
	SimpleMath::Vector2 localCenterGoal =  { diffGoal.Dot(naX), diffGoal.Dot(naY) };

	// ローカル座標系での最近点を求める
	SimpleMath::Vector2 localNearStart = { MyMath::Clamp(localCenterStart.x, -size.x, size.x),MyMath::Clamp(localCenterStart.y, -size.y, size.y), };
	SimpleMath::Vector2 localNearGoal  = { MyMath::Clamp(localCenterGoal.x, -size.x, size.x),MyMath::Clamp(localCenterGoal.y, -size.y, size.y), };

	// ワールド座標系へ戻す
	SimpleMath::Vector2 wStart = centerB + (naX * localNearStart.x) + (naY * localNearStart.y);
	SimpleMath::Vector2 wGoal  = centerB + (naX * localNearGoal.x) + (naY * localNearGoal.y);

	// ベクトルを作成
	SimpleMath::Vector2 vStart = wStart - worldPoints.first, vGoal = wGoal - worldPoints.second;

	// 正規化
	SimpleMath::Vector2 nvStart = vStart, nvGoal = vGoal;
	nvStart.Normalize(), nvGoal.Normalize();

	// このベクトルに対して判定

	// Start
	if (vStart.LengthSquared() > 0.0001f) {
		lA = abs(nvStart.Dot(halfLine)) + radius;
		lB = abs(nvStart.Dot(aX)) + abs(nvStart.Dot(aY));
		l = abs(nvStart.Dot(interval));
		if (lA + lB < l) return false;
		if ((lA + lB) - l < minOverLap)
		{
			minOverLap = (lA + lB) - l;
			minAxis = nvStart;
		}
	}

	// Goal
	if (vGoal.LengthSquared() > 0.0001f) {
		lA = abs(nvGoal.Dot(halfLine)) + radius;
		lB = abs(nvGoal.Dot(aX)) + abs(nvGoal.Dot(aY));
		l = abs(nvGoal.Dot(interval));
		if (lA + lB < l) return false;
		if ((lA + lB) - l < minOverLap)
		{
			minOverLap = (lA + lB) - l;
			minAxis = nvGoal;
		}
	}

	SimpleMath::Vector2 lineNormal = { -lineVec.y, lineVec.x }; // カプセルの法線
	lA = radius; // カプセルの「幅」
	lB = abs(lineNormal.Dot(aX)) + abs(lineNormal.Dot(aY)); // Boxの投影
	l = abs(lineNormal.Dot(interval));

	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = lineNormal;
	}

	// 全ての分離軸で衝突していれば

	// 衝突情報が必要な場合
	if (info)
	{
		// minAxisがAからBを指すように調整
		// DotがプラスならすでにBの方向を向いている
		if (minAxis.Dot(interval) > 0.0f)
		{
			minAxis *= -1.0f;
		}

		info->hitDir = minAxis;
		info->hitLen = minOverLap;
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();
	}

	return true;
}

bool CheckHit2D(BoxCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info)
{
	// 情報の取得
	// OBBの各軸2本×2の4本の分離軸に対して離れているか確認

	// 各軸を取得
	SimpleMath::Vector2 naX = colA->GetXAxis(), naY = colA->GetYAxis();
	SimpleMath::Vector2 nbX = colB->GetXAxis(), nbY = colB->GetYAxis();

	// 中心点を取得
	SimpleMath::Vector2 aCenter = colA->GetWorldCenterPos(), bCenter = colB->GetWorldCenterPos();
	SimpleMath::Vector2 interval = aCenter - bCenter;

	// サイズを取得
	SimpleMath::Vector2 aSize = colA->GetHalfSize(), bSize = colB->GetHalfSize();

	// 軸をサイズで掛ける
	SimpleMath::Vector2 aX = naX * aSize.x, aY = naY * aSize.y;
	SimpleMath::Vector2 bX = nbX * bSize.x, bY = nbY * bSize.y;

	// 投影後の長さ格納用変数
	float lA = 0, lB = 0, l = 0;

	// 最小の重なりとその軸
	float minOverLap = FLT_MAX;
	SimpleMath::Vector2 minAxis = { 0, 0 };

	// ----- Aの各軸を判定 ----- //

	// X軸
	lA = aSize.x;
	lB = abs(naX.Dot(bX)) + abs(naX.Dot(bY));
	l = abs(naX.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = naX;
	}

	// Y軸
	lA = aSize.y;
	lB = abs(naY.Dot(bX)) + abs(naY.Dot(bY));
	l = abs(naY.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = naY;
	}

	// ----- Bの各軸を判定 ----- //

	// X軸
	lA = abs(nbX.Dot(aX)) + abs(nbX.Dot(aY));
	lB = bSize.x;
	l = abs(nbX.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = nbX;
	}

	// Y軸
	lA = abs(nbY.Dot(aX)) + abs(nbY.Dot(aY));
	lB = bSize.y;
	l = abs(nbY.Dot(interval));
	if (lA + lB < l) return false;
	if ((lA + lB) - l < minOverLap)
	{
		minOverLap = (lA + lB) - l;
		minAxis = nbY;
	}

	// 全ての分離軸で衝突していれば

	// 衝突情報が必要な場合
	if (info)
	{
		// minAxisがAからBを指すように調整
		// DotがプラスならすでにBの方向を向いている
		if (minAxis.Dot(interval) > 0.0f)
		{
			minAxis *= -1.0f;
		}
		info->own = colA->GetOwn();
		info->target = colB->GetOwn();

		info->hitDir = minAxis;
		info->hitLen = minOverLap;
	}

	return true;
}

bool CheckHit2D(ConvexPolygonCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info)
{
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
	Projection aProj{}, bProj{};

	// MTVを調べる変数
	DirectX::SimpleMath::Vector2 mtvAxis;
	float minOverLap = FLT_MAX;

	// 各軸に対して分離しているか確認
	for (const auto& axis : axes)
	{
		// 2つの多角形を軸に投影
		aProj = Project(aVertices, axis);
		bProj = Project(bVertices, axis);

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

bool CheckHit2D(CircleCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info)
{
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

bool CheckHit2D(CapsuleCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info)
{
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
	size_t startNear = GetNearestPoint(vertices, capsule[0]), endNear = GetNearestPoint(vertices, capsule[1]);

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

	Projection capsuleProjection{}, polygonProjection;

	SimpleMath::Vector2 mtvAxis{};
	float minOverLap = FLT_MAX;

	float radius = colA->GetRadius();

	// 分離軸候補を調べる
	for (const auto& axis : axes)
	{
		capsuleProjection = Project(capsule, axis);
		polygonProjection = Project(vertices, axis);
		
		// 重なり量を調べる
		float overLap = GetOverlap(capsuleProjection, polygonProjection) + radius;

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
