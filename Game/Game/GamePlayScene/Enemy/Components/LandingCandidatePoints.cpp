//====================================================//
// ファイル名  : LandingCandidatePoints.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要       : コンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "LandingCandidatePoints.h"

#include "Components/World/Collider/3D/Shapes/Colliders.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void LandingCandidatePoints::Awake()
{}

void LandingCandidatePoints::Start()
{
	// コライダーを取得
	// 1つ目のみに対応
	m_ownCollider = static_cast<ColliderBase*>(GetOwn()->GetComponentWithCategory(Category::Collider));

	// 取得できなかったら何もしない
	if (!m_ownCollider) return;

	// 候補点の更新
	UpdateCandidatePoints();

	// 初期状態のバージョンを取得
	m_latestVersion = m_ownCollider->GetVersion();
}

void LandingCandidatePoints::Update(const GameTimer& gameTimer)
{
	gameTimer;

	// 再取得
	m_ownCollider = static_cast<ColliderBase*>(GetOwn()->GetComponentWithCategory(Category::Collider));

	// 再取得も失敗したら何もしない
	if (!m_ownCollider) return;

	// コライダーが変更されていたら
	if (m_ownCollider->GetVersion() != m_latestVersion)
	{
		// 最新バージョンを更新
		m_latestVersion = m_ownCollider->GetVersion();

		// 候補点の更新
		UpdateCandidatePoints();

		// 中心点の更新
		m_centerPoint = GetComponent<Transform>()->GetWorldPosition();
	}
}

void LandingCandidatePoints::UpdateCandidatePoints()
{
	// コライダーが未設定なら何もしない
	if (!m_ownCollider) return;

	// コライダーのタイプごとに処理を分ける
	switch (m_ownCollider->GetType())
	{
	case ColliderType::Box:
		UpdateCandidatePointsOnBox();
		break;
	case ColliderType::Capsule:
		break;
	case ColliderType::Sphere:
		break;
	default:
		break;
	}

	m_isChanged = true;
}

void LandingCandidatePoints::UpdateCandidatePointsOnBox()
{
	using namespace DirectX;

	// ボックスコライダーに変換
	BoxCollider* collider = static_cast<BoxCollider*>(m_ownCollider);

	// 変換に失敗したら何もしない
	if (!collider) return;

	// 最も上方向に近い軸を取得する
	SimpleMath::Vector3 axes[3] =
	{
		collider->GetXAxis(),
		collider->GetYAxis(),
		collider->GetZAxis()
	};

	// 軸は正規化済みのためそのまま使用
	float dots[3] =
	{
		SimpleMath::Vector3::UnitY.Dot(axes[0]),
		SimpleMath::Vector3::UnitY.Dot(axes[1]),
		SimpleMath::Vector3::UnitY.Dot(axes[2])
	};

	// 使う軸番号 (0:x 1:y 2:z)
	int useAxisIndex = -1;

	// 投影後の長さが最大となる軸を調べる
	float max = std::max(std::max(abs(dots[0]), abs(dots[1])), abs(dots[2]));
	for (int i = 0; i < 3; ++i) if (max == abs(dots[i])) useAxisIndex = i;

	if (useAxisIndex == -1) return;

	// 最大の軸が負だった場合は逆ベクトルを使用
	SimpleMath::Vector3 useAxis = axes[useAxisIndex];
	if (dots[useAxisIndex] < 0) useAxis *= -1;

	// ボックスのサイズを取得
	DirectX::SimpleMath::Vector3 size = collider->GetHalfSize();

	float sizes[3] = { size.x, size.y, size.z };

	// 決まった面の中心点と角の点の5点を候補とする
	m_candidatePoints.clear();

	SimpleMath::Vector3 center = collider->GetWorldCenterPos();

	// 面中心
	SimpleMath::Vector3 planeCenter = center + useAxis * sizes[useAxisIndex];

	m_candidatePoints.push_back(planeCenter);

	// 角
	int otherAxesIndex[2] { (useAxisIndex + 1) % 3, (useAxisIndex + 2) % 3 };

	float offset = 0.8f;

	// 1
	m_candidatePoints.push_back(
		planeCenter +
		axes[otherAxesIndex[0]] * sizes[otherAxesIndex[0]] * offset +
		axes[otherAxesIndex[1]] * sizes[otherAxesIndex[1]] * offset
	);
	// 2
	m_candidatePoints.push_back(
		planeCenter +
		-axes[otherAxesIndex[0]] * sizes[otherAxesIndex[0]] * offset +
		axes[otherAxesIndex[1]] * sizes[otherAxesIndex[1]] * offset
	);
	// 3
	m_candidatePoints.push_back(
		planeCenter +
		-axes[otherAxesIndex[0]] * sizes[otherAxesIndex[0]] * offset +
		-axes[otherAxesIndex[1]] * sizes[otherAxesIndex[1]] * offset
	);
	// 4
	m_candidatePoints.push_back(
		planeCenter +
		axes[otherAxesIndex[0]] * sizes[otherAxesIndex[0]] * offset +
		-axes[otherAxesIndex[1]] * sizes[otherAxesIndex[1]] * offset
	);
}
