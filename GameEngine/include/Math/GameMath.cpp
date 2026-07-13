//====================================================//
// ファイル名  : GameMath.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameMath.h"

//====================================================//
// 関数の実体宣言
//====================================================//

DirectX::SimpleMath::Quaternion Math::FromToRotation(DirectX::SimpleMath::Vector3 from, DirectX::SimpleMath::Vector3 to)
{
	// 正規化
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
