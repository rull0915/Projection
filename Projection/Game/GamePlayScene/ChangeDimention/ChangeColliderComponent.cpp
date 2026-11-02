/**
 * @file    ChangeColliderComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/02
 */

#include "pch.h"
#include "ChangeColliderComponent.h"

#include <array>

#include "Components/World/RigidBody/RigidBody.h"
#include "Components/World/RigidBody/RigidBody2D.h"
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"
#include "GameObject/GameObject.h"

using namespace RHEngine;

void ChangeColliderComponent::Change3DTo2D(CameraBase* pCamera)
{
	// 変換
	for (auto& component : GetOwn()->GetComponents<ColliderBase>())
	{
		auto collider = static_cast<ColliderBase*>(component);

		// 無効化
		collider->SetActive(false);

		// トリガーであれば生成しない
		if (collider->IsTrigger()) continue;

		// 2Dコライダーを生成
		Create2DColliderFrom3D(pCamera, collider);

		// 物理マテリアルを共有
		// cl2D->SetPhysicsMaterial(collider->GetPhysicsMaterial());
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

	owner->RemoveComponents<ColliderBase2D>();

	// 3Dコライダーを再有効化
	for (auto& component : GetOwn()->GetComponents<ColliderBase>())
	{
		component->SetActive(true);
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
	// 所有者を取得
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

	if (SphereCollider* sphere3D = dynamic_cast<SphereCollider*>(p3DCol))
	{
		// 円コライダーを生成する
		CircleCollider2D* circle2D = owner->AddComponent<CircleCollider2D>();

		// 半径を設定
		circle2D->SetRadius(sphere3D->GetRadius());

		// offset
		DirectX::SimpleMath::Vector3 offset = sphere3D->GetLocalCenterPos();

		circle2D->SetLocalPos(world3DToLocal2D(offset));

		// 生成したコライダーを返す
		return circle2D;
	}

	// カプセルのコライダーなら
	else if (CapsuleCollider* capsule3D = dynamic_cast<CapsuleCollider*>(p3DCol))
	{
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
		capsule2D->SetAxis(CapsuleCollider2D::AxisType2D::Horizontal);

		// offset
		DirectX::SimpleMath::Vector3 offset = capsule3D->GetLocalCenterPos();

		capsule2D->SetLocalPos(world3DToLocal2D(offset));

		return capsule2D;
	}

	// ボックスの場合 六角形を生成する
	else if (BoxCollider* box3D = dynamic_cast<BoxCollider*>(p3DCol))
	{
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

			if (num > 8 || num < 0) continue;

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

		// offset
		DirectX::SimpleMath::Vector3 offset = box3D->GetLocalCenterPos();

		collider->SetLocalPos(world3DToLocal2D(offset));

		return collider;
	}

	return nullptr;
}
