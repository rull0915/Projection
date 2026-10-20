//====================================================//
// ファイル名  : GameMath.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要       : 数学関数
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameMath.h"

//====================================================//
// 関数定義
//====================================================//

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
}
