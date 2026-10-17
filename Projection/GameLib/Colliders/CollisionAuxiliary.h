//====================================================//
// ファイル名   : CollisionAuxiliary.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要 : 衝突判定の補助ヘッダ
//
// 更新履歴 :
// 2026/06/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#define NOMINMAX
#include <vector>
#include <SimpleMath.h>
#include <algorithm>

//====================================================//
// クラス宣言
//====================================================//

namespace CollisionAuxiliary
{
	using namespace DirectX;

	// SAT用

	// 投影の最小値と最大値
	struct Projection
	{
		float min;
		float max;
	};

	// 投影する関数
	Projection Project(
		const std::vector<SimpleMath::Vector2>& poly,
		const SimpleMath::Vector2& axis)
	{
		// 最初の点を初期値として投影
		float min =
			poly[0].Dot(axis);
		float max = min;

		// 全ての点を調べて最小最大を更新
		for (auto& v : poly)
		{
			// 投影
			float p = v.Dot(axis);

			min = std::min(min, p);
			max = std::max(max, p);
		}

		return { min,max };
	}

	// 2つの投影情報から重なり量を調べる関数
	float GetOverlap(const Projection& p1, const Projection& p2)
	{
		float overlap = std::min(p1.max, p2.max) - std::max(p1.min, p2.min);
		return overlap;
	}

	// 点のリストの中で指定した点に最も近い点を取得する関数
	size_t GetNearestPoint(const std::vector<SimpleMath::Vector2>& points, const SimpleMath::Vector2& target)
	{
		if (points.size() <= 0) return 0;

		// 近い点番号
		size_t index = 0;

		float nearLenSq = (points[0] - target).LengthSquared();

		// 1番以降を調べる
		for (size_t i = 1; i < points.size(); ++i)
		{
			float lenSq = (points[i] - target).LengthSquared();

			if (lenSq < nearLenSq)
			{
				index = i;
				nearLenSq = lenSq;
			}
		}

		return index;
	}
}
