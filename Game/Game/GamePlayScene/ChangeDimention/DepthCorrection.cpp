//====================================================//
// ファイル名  : DepthCorrection.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/24
//
// 概要       : 2Dの際にZ軸の補正を行うコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DepthCorrection.h"

#include "Components/World/Transform/Transform.h"
#include "Physics/HitContact.h"

#include "Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void DepthCorrection::OnCollisionEnter2D(REngine::HitContact2D & contact)
{
	// 床にぶつかっていた場合
	if (contact.other->GetTag() == "Floor")
	{
		// 衝突法線がどれだけ上向きかを調べる
		float upVolume = DirectX::SimpleMath::Vector2::UnitY.Dot(contact.normal);

		// 衝突法線が上方向とみなせなければ何もしない
		if (upVolume > -CORRECTION_BORDER) return;

		// 衝突したオブジェクトに近づくように補正する

		// カメラ基準のZ座標を求める
		auto& world2D = REngine::WorldSetting2D::Instance();

		// 軸を取得する
		DirectX::SimpleMath::Vector3 xAxis = world2D.GetXAxis(), yAxis = world2D.GetYAxis();

		// 外積を使ってZ軸を算出
		DirectX::SimpleMath::Vector3 zAxis = xAxis.Cross(yAxis);
		zAxis.Normalize();

		// トランスフォームを取得
		REngine::Transform* targetTransform = contact.other->GetComponent<REngine::Transform>();
		REngine::Transform* ownTransform = GetComponent<REngine::Transform>();

		// 投影して座標を求める
		float targetZ = zAxis.Dot(targetTransform->GetWorldPosition());
		float ownZ = zAxis.Dot(ownTransform->GetWorldPosition());

		// 差分のみ座標を変化させる
		ownTransform->AddLocalPosition(zAxis * (targetZ - ownZ));
	}
}
