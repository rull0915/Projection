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
#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
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

	SceneCameraData CameraBase::CreateCameraData()
	{
		SceneCameraData data{};

		data.view = GetView();
		data.proj = GetProj();
		data.pos = GetComponent<Transform>()->GetWorldPosition();

		return data;
	}

	void CameraBase::DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color)
	{
		// 試錐台の範囲を表示する

		// VP逆行列から8頂点を求める
		DirectX::SimpleMath::Matrix invVP = (m_view * m_proj).Invert();

		DirectX::SimpleMath::Vector3 points[8]{};

		// 8頂点
		for (int i = 0; i < 8; i++)
		{
			DirectX::SimpleMath::Vector3 point =
			{
				i & 1 ? 1.0f : -1.0f,
				i & 2 ? 1.0f : -1.0f,
				i & 4 ? 1.0f :  0.0f
			};

			points[i] = DirectX::SimpleMath::Vector3::Transform(point, invVP);
		}

		// 合わせるように描画コマンドを作成
		renderer.Draw().Primitive().DrawRect(
			points[0], points[1], points[3], points[2], color, false
		);
		renderer.Draw().Primitive().DrawRect(
			points[4], points[5], points[7], points[6], color, false
		);
		for (int i = 0; i < 4; i++)
		{
			renderer.Draw().Primitive().DrawLine(
				points[i], points[i + 4], color
			);
		}
	}
}	// namespace RHEngine
