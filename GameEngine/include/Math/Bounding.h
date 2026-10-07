//====================================================//
// ファイル名   : Bounding.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/07
//
// 概要 : 境界となる形状をまとめたヘッダ
//
// 更新履歴 :
// 2026/10/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <SimpleMath.h>

namespace REngine
{
	/// <summary>
	/// 境界ボックス
	/// </summary>
	struct BoundingBox
	{
		DirectX::SimpleMath::Vector3 min;
		DirectX::SimpleMath::Vector3 max;

		BoundingBox(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b)
			: min{ a }
			, max{ b }
		{};

		BoundingBox()
			: min{ 0, 0, 0 }, max{ 0, 0, 0 }
		{}
	};

	/// <summary>
	/// 2D版境界ボックス
	/// </summary>
	struct BoundingBox2D
	{
		DirectX::SimpleMath::Vector2 min;
		DirectX::SimpleMath::Vector2 max;

		BoundingBox2D(DirectX::SimpleMath::Vector2 a, DirectX::SimpleMath::Vector2 b)
			: min{ a }
			, max{ b }
		{};

		BoundingBox2D()
			: min{ 0, 0 }, max{ 0, 0 }
		{}
	};

	/// <summary>
	/// 境界球
	/// </summary>
	struct BoundingSphere
	{
		DirectX::SimpleMath::Vector3 center;
		float radius;

		BoundingSphere(DirectX::SimpleMath::Vector3 c, float r)
			: center{ c }
			, radius{ r }
		{};

		BoundingSphere()
			: center{ 0, 0, 0 }, radius{ 0 }
		{}
	};
	
	/// <summary>
	/// 境界円
	/// </summary>
	struct BoundingCircle
	{
		DirectX::SimpleMath::Vector2 center;
		float radius;

		BoundingCircle(DirectX::SimpleMath::Vector2 c, float r)
			: center{ c }
			, radius{ r }
		{};

		BoundingCircle()
			: center{ 0, 0 }, radius{ 0 }
		{}
	};
}
