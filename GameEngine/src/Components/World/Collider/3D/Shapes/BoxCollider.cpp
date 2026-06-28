//====================================================//
// ファイル名  : BoxCollider.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/03
//
// 概要        :  ボックスのコライダー
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Collider/3D/Shapes/BoxCollider.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

void BoxCollider::UpdateCache() const
{
	Transform* pT = GetTransform();

	// ワールドの拡大率を取得
	SimpleMath::Vector3 worldScale = pT->GetWorldScale();

	// ----- 中心座標の更新 ----- //
	DirectX::SimpleMath::Vector3 world = SimpleMath::Vector3::Transform(GetLocalCenterPos(), pT->GetWorldMatrix());
	SetWorldPosition(world);

	// ----- 各軸の更新 ----- //
	m_cache.xAxis = pT->GetRight();
	m_cache.yAxis = pT->GetUp();
	m_cache.zAxis = pT->GetForward();

	// ----- スケールの更新 ----- //
	m_cache.scale = m_localSize * worldScale;

	// ----- AABBの更新 ----- //
	SimpleMath::Vector3 h = m_cache.scale * 0.5f;

	float ex = abs(m_cache.xAxis.x * h.x) + abs(m_cache.yAxis.x * h.y) + abs(m_cache.zAxis.x * h.z);
	float ey = abs(m_cache.xAxis.y * h.x) + abs(m_cache.yAxis.y * h.y) + abs(m_cache.zAxis.y * h.z);
	float ez = abs(m_cache.xAxis.z * h.x) + abs(m_cache.yAxis.z * h.y) + abs(m_cache.zAxis.z * h.z);

	SimpleMath::Vector3 extent(ex, ey, ez);

	// extentからmin maxを計算
	SetBoundingBox(AABB(world - extent, world + extent));

	// ----- ローカル座標行列の更新 ----- //
	m_cache.localMatrix = pT->GetWorldRotationMatrix() * SimpleMath::Matrix::CreateTranslation(world);

	m_cache.localMatrixInverse = SimpleMath::Matrix::CreateTranslation(-world) * pT->GetWorldRotationMatrix().Transpose();

	// フラグのリセット
	ResetDirty();
	SetChanged(true);

	ApplyVersion();
}
