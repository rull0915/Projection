//====================================================//
// ファイル名   : Origin.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要 : オリジン
//
// 更新履歴 :
// 2026/06/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <SimpleMath.h>

namespace Origin
{
	//====================================================//
	// 列挙型宣言
	//====================================================//
	enum class Type
	{
		RightTop,
		RightCenter,
		RightBottom,
		CenterTop,
		Center,
		CenterBottom,
		LeftTop,
		LeftCenter,
		LeftBottom,
	};

	inline static constexpr DirectX::SimpleMath::Vector2 GetPosition(Type originType)
	{
		switch (originType)
		{
		case Type::RightTop:
			return { 1, 0 };
		case Type::RightCenter:
			return { 1, 0.5f };
		case Type::RightBottom:
			return { 1, 1 };
		case Type::CenterTop:
			return { 0.5f, 0 };
		case Type::Center:
			return { 0.5f, 0.5f };
		case Type::CenterBottom:
			return { 0.5f, 1 };
		case Type::LeftTop:
			return { 0, 0 };
		case Type::LeftCenter:
			return { 0, 0.5f };
		case Type::LeftBottom:
			return { 0, 1 };
		default:
			return { 0, 0 };
		}
	}
}
