//====================================================//
// ファイル名  : ChangeColliderComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/02
//
// 概要       : コライダーの切り替えを行うコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include <array>

#include "ChangeColliderComponent.h"

#include "Components/World/RigidBody/RigidBody.h"
#include "Components/World/RigidBody/RigidBody2D.h"
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"

#include "GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ChangeColliderComponent::Change3DTo2D(CameraBase* pCamera)
{
	static std::vector<ComponentBase*> components{};

	// 3Dコライダーを取得
	GetOwn()->GetComponentsWithCategory(ComponentCategory::Collider, components);

	m_3dColliders.clear();

	// 変換
	for (auto& component : components)
	{
		m_3dColliders.push_back(static_cast<ColliderBase*>(component));
	}

	// 3Dコライダーをチェック
	for (auto& collider : m_3dColliders)
	{
		// 無効化
		collider->SetActive(false);

		// トリガーであれば生成しない
		if (collider->IsTrigger()) continue;

		// 2Dコライダーを生成
		auto cl2D = Create2DColliderFrom3D(pCamera, collider);

		// 物理マテリアルを共有
		cl2D->SetPhysicsMaterial(collider->GetMutablePhysicsMaterial());
	}

	// 3D物理挙動を無効化
	if (RigidBody* rid = GetOwn()->GetComponent<RigidBody>())
	{
		rid->SetActive(false);
		rid->SetVelocity(DirectX::SimpleMath::Vector3::Zero);

		static_cast<GameObject*>(GetOwn())->AddComponent<RigidBody2D>();// ->SetUseGravity(false);
	}
}

void ChangeColliderComponent::Change2DTo3D()
{
	// 2Dコライダーを削除する
	GameObject* owner = static_cast<GameObject*>(GetOwn());

	owner->RemoveComponentsWithCategory(ComponentCategory::Collider2D);

	// 3Dコライダーを有効化する
	for (auto& collider : m_3dColliders)
	{
		collider->SetActive(true);
	}

	// 2D物理挙動を削除する
	if (RigidBody* rid = GetOwn()->GetComponent<RigidBody>())
	{
		rid->SetActive(true);

		static_cast<GameObject*>(GetOwn())->RemoveComponents<RigidBody2D>();
	}	
}

ColliderBase2D* ChangeColliderComponent::Create2DColliderFrom3D(CameraBase* pCamera, ColliderBase* p3DCol)
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
			if (i % 4 == 2) bit++;
			if (i % 4 == 3) bit--;

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
			points[i].y *= -1;
		}

		// 最も中心に近い点とその反対側にある点を含めないようにする

		size_t index = 0;
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
		size_t exclude[2] = { index, ((index + 2) % 4) + 4 };

		// 2Dの六角形コライダーを生成
		ConvexPolygonCollider2D* collider = owner->AddComponent<ConvexPolygonCollider2D>();

		size_t num = exclude[0];

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

		// 必ず時計回りになるように補正
		collider->CorrectionClockWise();

		return collider;
	}
		break;
	default:
		break;
	}

	return nullptr;
}
