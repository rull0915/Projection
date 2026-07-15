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

namespace REngine
{
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
	}
}	// namespace REngine
