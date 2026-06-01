//====================================================//
// ファイル名  : BaseCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要       : カメラコンポーネント基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "BaseCamera.h"

#include "GameLib/GameObject/Components/Transform/Transform.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

Ray BaseCamera::GetRayToScreenPoint(const DirectX::SimpleMath::Vector2& point)
{
	// pointを中心基準の -1 ~ 1 座標系に変換
	float x = (2.0f * point.x) / Screen::WIDTH - 1.0f;
	float y = 1.0f - (2.0f * point.y) / Screen::HEIGHT;

	// 各逆行列で変換しワールド座標系に
	SimpleMath::Vector4 rayClip = { x, y, 1, 1 };

	// プロジェクションで変換しカメラ基準の3次元座標を求める
	SimpleMath::Vector4 rayEye = SimpleMath::Vector4::Transform(rayClip, GetInverseProj());

	// zをカメラから奥へ1進んだ場所に固定
	rayEye.z = -1;
	rayEye.w = 0;

	// viewで変換しワールド座標を求める
	SimpleMath::Vector4 rayWorld = SimpleMath::Vector4::Transform(rayEye, GetInverseView());

	// Vector3に戻す
	SimpleMath::Vector3 result = { rayWorld.x, rayWorld.y, rayWorld.z };

	// 念のため正規化
	result.Normalize();

	// スタート位置を取得
	DirectX::SimpleMath::Vector3 start = GetComponent<Transform>()->GetWorldPosition();

	// Rayを構築し返す
	return Ray(start, result);
}
