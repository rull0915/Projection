//====================================================//
// ファイル名  : ChangeColliderComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/02
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ChangeColliderComponent.h"

#include "GameLib/GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ChangeColliderComponent::Change3DTo2D(BaseCamera* pCamera)
{
	// 3Dコライダーを取得
	GetOwn()->GetComponents<BaseCollider>(m_3dColliders);

	// 3Dコライダーをチェック
	for (auto& collider : m_3dColliders)
	{
		// 無効化
		collider->SetActive(false);

		// 2Dコライダーを生成
		Create2DColliderFrom3D(pCamera, collider);
	}
}

void ChangeColliderComponent::Change2DTo3D()
{
	// 2Dコライダーを削除する
	GameObject* owner = static_cast<GameObject*>(GetOwn());

	owner->RemoveComponents<BaseCollider2D>();

	// 3Dコライダーを有効化する
	for (auto& collider : m_3dColliders)
	{
		collider->SetActive(true);
	}
}

BaseCollider2D* ChangeColliderComponent::Create2DColliderFrom3D(BaseCamera* pCamera, BaseCollider* p3DCol)
{
	// コライダータイプを取得
	ColliderType type3D = p3DCol->GetType();

	GameObject* owner = static_cast<GameObject*>(p3DCol->GetOwn());

	DirectX::SimpleMath::Vector3 xAxis = pCamera->GetInverseView().Right();
	DirectX::SimpleMath::Vector3 yAxis = pCamera->GetInverseView().Up();

	// 3次元から2次元へ座標変換するラムダ式
	auto world3DToLocal2D = [&xAxis, &yAxis](DirectX::SimpleMath::Vector3 point) -> DirectX::SimpleMath::Vector2
		{
			return
			{
				point.Dot(xAxis),
				point.Dot(yAxis)
			};
		};

	switch (type3D)
	{

		// 球の場合 円を生成する
	case ColliderType::Sphere:
	{
		// 球のコライダーにキャスト
		SphereCollider* sphere3D = static_cast<SphereCollider*>(p3DCol);

		// そのまま円を生成する
		CircleCollider2D* circle2D = owner->AddComponent<CircleCollider2D>();

		// 半径を設定
		circle2D->SetRadius(sphere3D->GetRadius());

		// 生成したコライダーを返す
		return circle2D;
	}

		// カプセルの場合 カプセルを生成する
	case ColliderType::Capsule:
	{
		// カプセルのコライダーにキャスト
		CapsuleCollider* capsule3D = static_cast<CapsuleCollider*>(p3DCol);

		// 2点を2D座標に変換
		std::pair<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3> points3D = capsule3D->GetPoints();
		std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> points2D =
		{
			world3DToLocal2D(points3D.first), world3DToLocal2D(points3D.second)
		};

		// 2点から軸ベクトルを求める
		DirectX::SimpleMath::Vector2 axis2D = points2D.second - points2D.first;
		axis2D.Normalize();

		// 軸ベクトルがX軸からどれくらい回転しているかを求める
		float angle = std::atan2f(axis2D.y, axis2D.x);

		// 半径はそのまま使用する
		float radius = capsule3D->GetRadius();

		// 2点からカプセルの高さを求める
		float height = (points2D.second - points2D.first).Length() + radius * 2;

		CapsuleCollider2D* capsule2D = owner->AddComponent<CapsuleCollider2D>();

		// 設定
		capsule2D->SetRadius(radius);
		capsule2D->SetHeight(height);
		capsule2D->SetRotation(angle);
		capsule2D->SetLineDir(AxisType2D::Horizontal);

		return capsule2D;
	}

		// ボックスの場合 六角形を生成する
	case ColliderType::Box:
	{
		BoxCollider* box3D = static_cast<BoxCollider*>(p3DCol);

		// ボックスの情報を取得
		DirectX::SimpleMath::Vector3 center3D = box3D->GetWorldCenterPos();

		DirectX::SimpleMath::Vector3
			xLocalAxis = box3D->GetXAxis(),
			yLocalAxis = box3D->GetYAxis(),
			zlocalAxis = box3D->GetZAxis();

		// 角の8つ、中心の1つを投影した9点を取得
		std::array<DirectX::SimpleMath::Vector2, 8> points;

		for (size_t i = 0; i < 8; ++i)
		{
			// 1週する順番になるように2,3を入れ替える
			size_t bit = i;
			if (i % 4 == 0) bit++;
			if (i % 4 == 1) bit--;

			// 3bitを各軸が+かどうかとして扱う
			bool xPositive = bit & 0b100;
			bool yPositive = bit & 0b010;
			bool zPositive = bit & 0b001;

			DirectX::SimpleMath::Vector3 halfSize = box3D->GetHalfSize();

			// 点を生成
			DirectX::SimpleMath::Vector3 point3D =
				(xPositive ? xLocalAxis : -xLocalAxis) * halfSize.x +
				(yPositive ? yLocalAxis : -yLocalAxis) * halfSize.y +
				(zPositive ? zlocalAxis : -zlocalAxis) * halfSize.z;

			// 2Dに投影
			points[i] = world3DToLocal2D(point3D);

			//
			points[i].y *= -1;
		}

		// 最も中心に近い点とその反対側にある点を含めないようにする

		int index = -1;
		float min = FLT_MAX;

		// 0~3まで回せばZ座標が同じ4点を調べられる
		for (size_t i = 0; i < 4; ++i)
		{
			float lenSq = points[i].LengthSquared();

			if (lenSq < min)
			{
				min = lenSq;
				index = i;
			}
		}

		// 含めない2点のインデックスを算出
		int exclude[2] = { index, ((index + 2) % 4) + 4 };

		// 2Dの六角形コライダーを生成
		ConvexPolygonCollider2D* collider = owner->AddComponent<ConvexPolygonCollider2D>();

		int num = exclude[0];

		// 最初の3個
		for (size_t i = 0; i < 3; ++i)
		{
			num = (num + 1) % 4;

			collider->AddVertex(points[num]);
		}
		
		// 次の3個
		for (size_t i = 0; i < 3; ++i)
		{
			collider->AddVertex(points[num + 4]);
			num = (num + 1) % 4;
		}

		return collider;
	}
		break;
	default:
		break;
	}

	return nullptr;
}
