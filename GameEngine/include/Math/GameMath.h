//====================================================//
// ファイル名   : GameMath.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要 : 数学関数をまとめたヘッダ
//
// 更新履歴 :
// 2026/06/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>
#include <SimpleMath.h>

//====================================================//
// 関数宣言
//====================================================//

namespace Math
{
	// 符号取得関数
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	int Sign(T value)
	{
		if (value > 0) return 1;
		if (value < 0) return -1;
		return 0;
	}

	// ベクトルを特定のベクトルに変換するクォータニオンを返す関数
	DirectX::SimpleMath::Quaternion FromToRotation(DirectX::SimpleMath::Vector3 from, DirectX::SimpleMath::Vector3 to);
}
