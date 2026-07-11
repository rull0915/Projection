//====================================================//
// ファイル名   : GameMath.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : 数学関数
//
// 更新履歴 : 
// 2026/06/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

// 関数宣言

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
}
