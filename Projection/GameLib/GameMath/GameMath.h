/**
 * @file    GameMath.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   数学関数
 */
#pragma once

namespace MyMath
{
	// 線
	struct Line
	{
		DirectX::SimpleMath::Vector2 position1;
		DirectX::SimpleMath::Vector2 position2;
	};

	// 交点を求める
	DirectX::SimpleMath::Vector2 GetIntersection(Line line1, Line line2);

	
	// ベクトルを特定のベクトルに変換するクォータニオンを返す関数
	DirectX::SimpleMath::Quaternion FromToRotation(DirectX::SimpleMath::Vector3 from, DirectX::SimpleMath::Vector3 to);
}
