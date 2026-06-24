//====================================================//
// ファイル名   : CheckHitAuxiliary.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/04
//
// 概要 : 衝突判定で使用する補助関数をまとめたヘッダです
//
// 更新履歴 :
// 2026/04/04 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// 関数宣言
//====================================================//

/// <summary>
/// 線分(p1, p2)上で、点pに最も近い点を探す補助関数
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p"></param>
/// <returns></returns>
static DirectX::SimpleMath::Vector3 ClosestPointOnSegment(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& p)
{
    DirectX::SimpleMath::Vector3 v = p2 - p1;    // 線分の方向ベクトル
    DirectX::SimpleMath::Vector3 w = p - p1;     // 点からp1への方向ベクトル

    // 線分の長さの2乗
    float l = v.LengthSquared();
    if (l < 0.0001f) return p1; // 点に近い線分の場合

    // 投影比率 t を内積から計算
    float t = w.Dot(v) / l;

    // 線分の範囲内にクランプ
    t = std::clamp(t, 0.0f, 1.0f);

    // 線分上の最近接点
    return p1 + v * t;
}

/// <summary>
/// 線分同士の最短距離を求める関数
/// </summary>
static float ClosedSegmentToSegment(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& q1, const DirectX::SimpleMath::Vector3& q2, DirectX::SimpleMath::Vector3& outP, DirectX::SimpleMath::Vector3& outQ)
{
    // 線分のベクトル
    DirectX::SimpleMath::Vector3 vP = p2 - p1, vQ = q2 - q1;
    DirectX::SimpleMath::Vector3 PtoQ = p1 - q1;

    // 線分の長さの2乗を求める
    float sqLP = vP.Dot(vP), sqLQ = vQ.Dot(vQ);

    // 2直線の外積
    float dt = vP.Dot(vQ);
    float den = sqLP * sqLQ - dt * dt; // 行列式

    // 線分が短い場合
    if (sqLP < 0.001f)
    {
        outP = p1, outQ = ClosestPointOnSegment(q1, q2, p1);
        return (outP - outQ).LengthSquared();
    }
    if (sqLQ < 0.001f)
    {
        outP = ClosestPointOnSegment(p1, p2, q1), outQ = q1;
        return (outP - outQ).LengthSquared();
    }

    // 平行の場合
    if (den < 0.0001f)
    {
        // p1が最近点だと仮定
        // 投影比率 t を内積から計算
        float tP = 0, tQ = PtoQ.Dot(vQ) / sqLQ;

        // 線分内の場合
        if (tQ >= 0 && tQ <= 1)
        {
            outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
            return (outP - outQ).LengthSquared();
        }
        // さらに線分外なら
        tQ = std::clamp(tQ, 0.0f, 1.0f);   // Qを0 ~ 1にクランプ

		DirectX::SimpleMath::Vector3 pQ = q1 + tQ * vQ;

		// pQから線分pへの最近点を求める
		tP = ((pQ - p1).Dot(vP)) / sqLP;

        // 線分内なら
        if (tP >= 0 && tP <= 1) 
        {
            outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
            return (outP - outQ).LengthSquared();
        }

        tP = std::clamp(tP, 0.0f, 1.0f);   // Pを0 ~ 1にクランプ
        outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
        return (outP - outQ).LengthSquared();
    }
    // 平行じゃない場合
    else
    {
        // 2直線の最近点2つを求める
        float tP = (dt * vQ.Dot(PtoQ) - sqLQ * vP.Dot(PtoQ)) / den;
        float tQ = (vQ.Dot(PtoQ + tP * vP)) / sqLQ;

        // 2点の状況で場合分け
        // どちらも線分内なら
        if ((tP >= 0 && tP <= 1) && (tQ >= 0 && tQ <= 1))
        {
            outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
            return (outP - outQ).LengthSquared();
        }
        // どちらかが線分外なら
        tP = std::clamp(tP, 0.0f, 1.0f);   // Pを0 ~ 1にクランプ

        // クランプ後の点から最近点を求める
        DirectX::SimpleMath::Vector3 pP = p1 + tP * vP;
        DirectX::SimpleMath::Vector3 w = pP - q1;     // 点からp1への方向ベクトル

        // 投影比率 t を内積から計算
        tQ = w.Dot(vQ) / sqLQ;

        // 線分内の場合
        if (tQ >= 0 && tQ <= 1)
        {
            outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
            return (outP - outQ).LengthSquared();
        }
        // さらに線分外なら
        tQ = std::clamp(tQ, 0.0f, 1.0f);   // Qを0 ~ 1にクランプ

        outP = p1 + tP * vP; outQ = q1 + tQ * vQ;
        return (outP - outQ).LengthSquared();
    }
}

static DirectX::SimpleMath::Vector3 ClosedPointOnAABB(const DirectX::SimpleMath::Vector3& halfSize, const DirectX::SimpleMath::Vector3 point)
{
    DirectX::SimpleMath::Vector3 nearPoint = {
    std::clamp(point.x, -halfSize.x, halfSize.x),
    std::clamp(point.y, -halfSize.y, halfSize.y),
    std::clamp(point.z, -halfSize.z, halfSize.z)
    };

    return nearPoint;
}

inline static float LengthSegmentOnSeparateAxis(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& x, const DirectX::SimpleMath::Vector3& y, const DirectX::SimpleMath::Vector3& z)
{
    return abs(target.Dot(x)) + abs(target.Dot(y)) + abs(target.Dot(z));
}

// 球と線分の交点を求める(始点に近い方)
static bool GetIntersectionSphereAndLine(DirectX::SimpleMath::Vector3 center, float radius, DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float& outT, DirectX::SimpleMath::Vector3& outP)
{
    DirectX::SimpleMath::Vector3 v = end - start, oc = start - center;

    float a = v.Dot(v);

    float b = 2 * (v.Dot(oc));

    float c = oc.Dot(oc) - radius * radius;

    // 判定式を使い当たる可能性があるか調べる
    float D = b * b - 4 * a * c;

    // 衝突しない
    if (D < 0) return false;

    // 衝突する場合、衝突点を求める
    float sqD = std::sqrtf(D);
    float t1 = (-b - sqD) / (2 * a), t2 = (-b + sqD) / (2 * a);

    // 線分内に点があるかを調べる一時関数    
    auto valid =
        [&](float value)
        {
            return
                value >= 0.0f &&
                value <= 1.0f;
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

    // 出力用引数に代入
    outT = t;
    outP = start + v * t;

    return true;
}
