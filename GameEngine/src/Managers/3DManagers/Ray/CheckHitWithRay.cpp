//====================================================//
// ファイル名  : CheckHitWithRay.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/31
//
// 概要       : レイとコライダーの衝突を調べる関数群
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CheckHitWithRay.h"

#include "../Collider/CheckHit/CheckHitAuxiliary.h"

#include "Components/World/Collider/3D/Shapes/BoxCollider.h"
#include "Components/World/Collider/3D/Shapes/SphereCollider.h"
#include "Components/World/Collider/3D/Shapes/CapsuleCollider.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RayCollision
{
	bool RayCollision::CheckHit_Ray_Sphere(const Ray& ray, float distance, ColliderBase* co, RaycastHit* hit)
	{
		// コライダーを変換
		SphereCollider* col = static_cast<SphereCollider*>(co);

		// 球と線の連立方程式を解く方法を使用
		DirectX::SimpleMath::Vector3 center = col->GetWorldCenterPos();
		float radius = col->GetRadius();

		// 2次方程式の解の公式のa,b,cに当たる部分を求める。

		DirectX::SimpleMath::Vector3 oc = ray.GetStart() - center;  // 球中心からレイの原点に向かうベクトル

		// aはdirectionが正規化前提のため1になる。

		float b = ray.GetDirection().Dot(oc);

		float c = oc.Dot(oc) - radius * radius;

		// 判定式を使い当たる可能性があるか調べる
		float D = b * b - c;

		// 衝突しない
		if (D < 0) return false;

		// 衝突する場合、衝突点を求める
		float sqD = std::sqrtf(D);
		float t1 = -b - sqD, t2 = -b + sqD;

		// 線分内に点があるかを調べる一時関数    
		auto valid =
			[&](float value)
			{
				return
					value >= 0.0f &&
					value <= distance;
			};

		float t = FLT_MAX;

		// 線分内にある中で最も始点に近い点を求める
		if (valid(t1))
		{
			t = t1;
		}
		if (valid(t2))
		{
			t = std::min(t, t2);
		}
		// どちらも線分外だった場合
		if (t == FLT_MAX) return false;

		// 衝突情報に代入する
		hit->distance = t;
		hit->point = ray.GetStart() + t * ray.GetDirection();
		hit->collider = col;
		hit->normal = hit->point - center;
		hit->normal.Normalize();

		return true;
	}

	bool CheckHit_Ray_Capsule(const Ray& ray, float distance, ColliderBase* co, RaycastHit* hit)
	{
		// コライダーを変換
		CapsuleCollider* col = static_cast<CapsuleCollider*>(co);

		// 情報を取得

		// カプセルの中心線分の2点
		DirectX::SimpleMath::Vector3 sC = col->GetPoints().first, eC = col->GetPoints().second;
		// rayを構成する2点
		DirectX::SimpleMath::Vector3 sR = ray.GetStart(), eR = sR + ray.GetDirection() * distance;
		// rayのベクトル
		DirectX::SimpleMath::Vector3 v = eR - sR;
		// 半径
		float radius = col->GetRadius();
		// 中心線分の長さ
		float height = col->GetLineLength();

		// -------------------------------------------------------------
		// 0. 両端の球用の連立方程式を解くラムダ式
		// -------------------------------------------------------------
		auto Valid = [](float p) -> bool { return p >= 0 && p <= 1; };
		// 特定の球とRayの交点となる媒介変数tを2つとも計算し、有効な方を選択する
		auto SolveSphere = [&](const DirectX::SimpleMath::Vector3& center, float originalT) -> float
			{
				DirectX::SimpleMath::Vector3 oc = sR - center;

				float aSq = v.Dot(v);
				if (aSq < FLT_EPSILON) return FLT_MAX;
				float bSq = 2.0f * v.Dot(oc);
				float cSq = oc.Dot(oc) - radius * radius;
				float dSq = bSq * bSq - 4.0f * aSq * cSq;
				if (dSq < 0.0f) return FLT_MAX;

				float sD = std::sqrtf(dSq);
				float tSph1 = (-bSq - sD) / (2.0f * aSq);
				float tSph2 = (-bSq + sD) / (2.0f * aSq);

				// 円筒の判定から流れてきた元のtに近い方を選択する
				bool v1 = Valid(tSph1);
				bool v2 = Valid(tSph2);

				if (v1 && v2) return (std::abs(tSph1 - originalT) < std::abs(tSph2 - originalT)) ? tSph1 : tSph2;
				if (v1) return tSph1;
				if (v2) return tSph2;
				return FLT_MAX;
			};


		// -------------------------------------------------------------
		// 1. 中心線分が0の場合の特殊処理 (球として扱う)
		// -------------------------------------------------------------
		if (height <= FLT_EPSILON)
		{
			float t = SolveSphere(sC, 0);

			// 衝突情報に代入する
			hit->distance = t * distance;
			hit->point = ray.GetStart() + t * distance * ray.GetDirection();
			hit->collider = col;
			hit->normal = hit->point - sC;
			hit->normal.Normalize();

			return true;
		}

		// -------------------------------------------------------------
		// 2. 無限円筒とRayとの交差判定
		// -------------------------------------------------------------

		// 無限円筒とRayとの交点を求める。
		DirectX::SimpleMath::Vector3 u = eC - sC;
		DirectX::SimpleMath::Vector3 w = sR - sC;

		DirectX::SimpleMath::Vector3 d1 = u.Cross(w);
		DirectX::SimpleMath::Vector3 d2 = u.Cross(v);

		// 2次方程式を構成する各要素を求める
		float A = d2.Dot(d2);

		// 2点の媒介変数を用意
		float t[2] = { FLT_MAX, FLT_MAX };

		// 線分が平行の場合の処理
		if (A < FLT_EPSILON)
		{
			// 両端球判定に任せるためにtをエラー値のままに
		}
		else
		{
			float B = 2 * (d1.Dot(d2));
			float C = d1.Dot(d1) - (radius * radius) * u.Dot(u);

			// 判別式Dを作成
			float D = B * B - 4 * A * C;

			// 無限円筒と線分が交差しない場合はfalse
			if (D < 0) return false;

			// 2点の媒介変数tを求める
			float sqrtD = std::sqrtf(D);
			t[0] = (-B - sqrtD) / (2 * A), t[1] = (-B + sqrtD) / (2 * A);
		}

		// 最終的な2つの交点座標となる媒介変数t
		float finalT[2]{ FLT_MAX, FLT_MAX };

		// 2点が有効かどうかのフラグ
		bool isValid[2] = { false, false };

		// -------------------------------------------------------------
		// 3. 2つの交点候補の検証
		// -------------------------------------------------------------

		for (int i = 0; i < 2; i++)
		{
			// そもそもRayの線分範囲外なら円筒としても球としても交点になり得ない
			if (!Valid(t[i])) continue;

			// 点が非無限円筒内にあるかを調べる
			DirectX::SimpleMath::Vector3 p = sR + v * t[i];

			// カプセルの中心線分に投影する
			float s = (p - sC).Dot(u) / u.Dot(u);

			// sが０~１の場合カプセルの円筒に衝突しているため交点が確定する
			if (Valid(s))
			{
				finalT[i] = t[i];
				isValid[i] = true;
				continue;
			}

			// そうでない場合、両端の球との連立方程式を解く

			// 0未満の場合、始点中心の球 1超過の場合、終点中心の球
			DirectX::SimpleMath::Vector3 sphereCenter = (s < 0.0f) ? sC : eC;
			float sphereT = SolveSphere(sphereCenter, t[i]);

			// 衝突する場合
			if (sphereT != FLT_MAX)
			{
				finalT[i] = sphereT;
				isValid[i] = true;
			}
		}

		// -------------------------------------------------------------
		// 4. 最も始点に近い交点を選択して結果を出力
		// -------------------------------------------------------------

		// 最終的に求まった2点の中で始点に近い方を選択
		float T = FLT_MAX;

		if (isValid[0]) T = finalT[0];

		if (isValid[1]) T = std::min(T, finalT[1]);

		if (T == FLT_MAX) return false;

		// 最終的な交点を求める
		DirectX::SimpleMath::Vector3 intersection = sR + v * T;

		// 衝突情報を格納
		hit->collider = col;
		hit->distance = distance * T;

		// カプセル線分上の交点への最近点を求める
		DirectX::SimpleMath::Vector3 nearP = ClosestPointOnSegment(sC, eC, intersection);
		hit->normal = intersection - nearP;
		hit->normal.Normalize();

		hit->point = intersection;

		return true;
	}

	bool CheckHit_Ray_Box(const Ray& ray, float distance, ColliderBase* co, RaycastHit* hit)
	{
		// コライダーを変換
		BoxCollider* col = static_cast<BoxCollider*>(co);

		// ボックスのローカル座標系にrayの2点を変換する

		// ボックスのワールド行列の逆行列を使って、球の中心をローカル座標へ
		DirectX::SimpleMath::Matrix worldToLocal = col->GetLocalMatrixInverse();

		DirectX::SimpleMath::Vector3 wS = ray.GetStart();
		DirectX::SimpleMath::Vector3 wE = wS + distance * ray.GetDirection();
		DirectX::SimpleMath::Vector3 lS = DirectX::SimpleMath::Vector3::Transform(wS, worldToLocal);
		DirectX::SimpleMath::Vector3 lE = DirectX::SimpleMath::Vector3::Transform(wE, worldToLocal);

		DirectX::SimpleMath::Vector3 halfSize = col->GetHalfSize();

		DirectX::SimpleMath::Vector3 minPos = -halfSize, maxPos = halfSize;

		float tMin = 0.0f;          // レイの始点
		float tMax = 1.0f;          // レイの終点

		DirectX::SimpleMath::Vector3 lRayDir = lE - lS;

		// tMinが使われた軸を保持しておく(0: x, 1: y, 2: z)
		int hitAxis = -1;

		// --- X軸の判定 ---
		if (std::abs(lRayDir.x) > FLT_EPSILON) {
			float tx1 = (minPos.x - lS.x) / lRayDir.x;
			float tx2 = (maxPos.x - lS.x) / lRayDir.x;

			float minTx = std::min(tx1, tx2);
			if (minTx > tMin) {
				tMin = minTx;
				hitAxis = 0; // X面でtMinが更新された
			}
			tMax = std::min(tMax, std::max(tx1, tx2));
		}
		else if (lS.x < minPos.x || lS.x > maxPos.x) return false;
		// --- Y軸の判定 ---
		if (std::abs(lRayDir.y) > FLT_EPSILON) {
			float ty1 = (minPos.y - lS.y) / lRayDir.y;
			float ty2 = (maxPos.y - lS.y) / lRayDir.y;

			float minTy = std::min(ty1, ty2);
			if (minTy > tMin) {
				tMin = minTy;
				hitAxis = 1; // Y面でtMinが更新された
			}
			tMax = std::min(tMax, std::max(ty1, ty2));
		}
		else if (lS.y < minPos.y || lS.y > maxPos.y) return false;
		// --- Z軸の判定 ---
		if (std::abs(lRayDir.z) > FLT_EPSILON) {
			float tz1 = (minPos.z - lS.z) / lRayDir.z;
			float tz2 = (maxPos.z - lS.z) / lRayDir.z;

			float minTz = std::min(tz1, tz2);
			if (minTz > tMin) {
				tMin = minTz;
				hitAxis = 2; // Z面でtMinが更新された
			}
			tMax = std::min(tMax, std::max(tz1, tz2));
		}
		else if (lS.z < minPos.z || lS.z > maxPos.z) return false;

		// 範囲が0の場合
		if (tMin > tMax) return false;

		// 範囲があれば衝突している minが始点に近いためそちらを採用
		DirectX::SimpleMath::Vector3 localIntersection = lS + lRayDir * tMin;

		// ワールドに変換
		DirectX::SimpleMath::Vector3 intersection = DirectX::SimpleMath::Vector3::Transform(localIntersection, col->GetLocalMatrix());

		hit->collider = col;
		// スケールなし行列変換のためtはそのまま使用可能
		hit->distance = tMin * distance;
		hit->point = intersection;

		// 法線を求める

		// 変換前の交点の各軸座標に応じて決める
		switch (hitAxis)
		{
		case -1:
			hit->normal = -ray.GetDirection();
			break;
		case 0:
			hit->normal = localIntersection.x > 0 ? col->GetXAxis() : -col->GetXAxis();
			break;
		case 1:
			hit->normal = localIntersection.y > 0 ? col->GetYAxis() : -col->GetYAxis();
			break;
		case 2:
			hit->normal = localIntersection.z > 0 ? col->GetZAxis() : -col->GetZAxis();
			break;
		default:
			break;
		}

		hit->normal.Normalize();
		return true;
	}
}
