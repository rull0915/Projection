/**
 * @file    GameMath.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 */

#include "pch.h"
#include "GameMath.h"

namespace MyMath
{
	DirectX::SimpleMath::Vector2 GetIntersection(Line line1, Line line2)
	{
		// xの分母
		double denominator = (line2.position1.y - line2.position2.y) * (line1.position2.x - line1.position1.x) - (line1.position1.y - line1.position2.y) * (line2.position2.x - line2.position1.x);

		// 分子
		double molecule1 = (line1.position1.x * line1.position2.y - line1.position1.y * line1.position2.x) * (line2.position2.x - line2.position1.x);
		double molecule2 = (line2.position1.x * line2.position2.y - line2.position1.y * line2.position2.x) * (line1.position2.x - line1.position1.x);

		// x 
		double x = (molecule1 - molecule2) / denominator;

		// y
		double y = ((line1.position1.y - line1.position2.y) * (x - line1.position1.x) / (line1.position1.x - line1.position2.x)) + line1.position1.y;
		if ((line1.position1.x - line1.position2.x) == 0)
		{
			y = ((line2.position1.y - line2.position2.y) * (x - line2.position1.x) / (line2.position1.x - line2.position2.x)) + line2.position1.y;
		}

		if (abs(y) < 0.001)
		{
			y = 0;
		}

		return DirectX::SimpleMath::Vector2{ static_cast<float>(x), static_cast<float>(y) };
	}

	DirectX::SimpleMath::Quaternion FromToRotation(DirectX::SimpleMath::Vector3 from, DirectX::SimpleMath::Vector3 to)
	{	// 正規化
		from.Normalize();
		to.Normalize();

		// 外積を用いて回転軸を求める
		DirectX::SimpleMath::Vector3 cross = from.Cross(to);

		if (cross == DirectX::SimpleMath::Vector3::Zero) return DirectX::SimpleMath::Quaternion::Identity;

		// 正規化
		cross.Normalize();

		// 内積を用いて回転角を求める
		float dot = from.Dot(to);

		// アークコサインでコサインの値から角度を算出
		float angle = std::acosf(dot);

		// クォータニオンを返す
		return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(cross, angle);
	}
}	// namespace MyMath
