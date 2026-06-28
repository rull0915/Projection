//====================================================//
// ファイル名  : CameraBase.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要       : カメラコンポーネント基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Camera/CameraBase.h"

#include "Components/World/Transform/Transform.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

Ray CameraBase::GetRayToScreenPoint(const DirectX::SimpleMath::Vector2& point)
{
	// pointを中心基準の -1 ~ 1 座標系に変換
	float x = (2.0f * point.x) / WindowManager::Instance().GetWidth() - 1.0f;
	float y = 1.0f - (2.0f * point.y) / WindowManager::Instance().GetHeight();

	// 各逆行列で変換しワールド座標系に
	DirectX::SimpleMath::Vector4 rayClip = { x, y, 1, 1 };

	// プロジェクションで変換しカメラ基準の3次元座標を求める
	DirectX::SimpleMath::Vector4 rayEye = DirectX::SimpleMath::Vector4::Transform(rayClip, GetInverseProj());

	// zをカメラから奥へ1進んだ場所に固定
	rayEye.z = -1;
	rayEye.w = 0;

	// viewで変換しワールド座標を求める
	DirectX::SimpleMath::Vector4 rayWorld = DirectX::SimpleMath::Vector4::Transform(rayEye, GetInverseView());

	// Vector3に戻す
	DirectX::SimpleMath::Vector3 result = { rayWorld.x, rayWorld.y, rayWorld.z };

	// 念のため正規化
	result.Normalize();

	// スタート位置を取得
	DirectX::SimpleMath::Vector3 start = GetComponent<Transform>()->GetWorldPosition();

	// Rayを構築し返す
	return Ray(start, result);
}
