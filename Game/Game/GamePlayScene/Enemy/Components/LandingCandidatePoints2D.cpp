//====================================================//
// ファイル名  : LandingCandidatePoints2D.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要       : 2D版の着地候補点コンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "LandingCandidatePoints2D.h"

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "GameLib/Colliders/ConvexPolygonCollider2D.h"
#include "Common/TypeIdGenerator.h"

#include "Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void LandingCandidatePoints2D::Awake()
{}

void LandingCandidatePoints2D::Start()
{
	// コライダーを取得
	// 1つ目のみに対応
	m_ownCollider = static_cast<REngine::ColliderBase2D*>(GetOwn()->GetComponent<REngine::ColliderBase2D>());

	// 取得できなかったら何もしない
	if (!m_ownCollider) return;

	// 候補点の更新
	UpdateCandidatePoints();

	// 初期状態のバージョンを取得
	m_latestVersion = m_ownCollider->GetVersion();
}

void LandingCandidatePoints2D::Update(const REngine::GameTimer& gameTimer)
{
	gameTimer;

	// 再取得
	m_ownCollider = static_cast<REngine::ColliderBase2D*>(GetOwn()->GetComponent<REngine::ColliderBase2D>());

	// 再取得も失敗したら何もしない
	if (!m_ownCollider) return;

	// コライダーが変更されていたら
	if (m_ownCollider->GetVersion() != m_latestVersion)
	{
		// 最新バージョンを更新
		m_latestVersion = m_ownCollider->GetVersion();

		// 候補点の更新
		UpdateCandidatePoints();

		// 2D世界の設定を取得
		auto& world2D = REngine::WorldSetting2D::Instance();

		// 中心点の更新(トランスフォームの3D座標を2D座標系に変換)
		m_centerPoint = world2D.World3DToLocal2D(GetComponent<REngine::Transform>()->GetWorldPosition());
	}
}

void LandingCandidatePoints2D::UpdateCandidatePoints()
{
	// コライダーが未設定なら何もしない
	if (!m_ownCollider) return;

	// コライダーのタイプごとに処理を分ける

	// ボックスの場合
	if (m_ownCollider->IsTypeOf(REngine::BoxCollider2D::StaticTypeId))
	{
		UpdateCandidatePointsOnBox();
	}
	// カプセル
	else if (m_ownCollider->IsTypeOf(REngine::CapsuleCollider2D::StaticTypeId))
	{

	}
	// 円
	else if (m_ownCollider->IsTypeOf(REngine::CircleCollider2D::StaticTypeId))
	{

	}
	// 凸ポリゴン
	else if (m_ownCollider->IsTypeOf(ConvexPolygonCollider2D::StaticTypeId))
	{
		UpdateCandidatePointsOnConvexPolygon();
	}

	m_isChanged = true;
}

void LandingCandidatePoints2D::UpdateCandidatePointsOnBox()
{
	using namespace DirectX;

	// ボックスコライダーに変換
	REngine::BoxCollider2D* collider = dynamic_cast<REngine::BoxCollider2D*>(m_ownCollider);

	// 変換に失敗したら何もしない
	if (!collider) return;

	// 最も上方向に近い軸を取得する
	SimpleMath::Vector2 axes[2] =
	{
		collider->GetXAxis(),
		collider->GetYAxis(),
	};

	// 軸は正規化済みのためそのまま使用
	float dots[2] =
	{
		SimpleMath::Vector2::UnitY.Dot(axes[0]),
		SimpleMath::Vector2::UnitY.Dot(axes[1]),
	};

	// 使う軸番号 (0:x 1:y)
	int useAxisIndex = -1;

	// 投影後の長さが最大となる軸を調べる
	float max = std::max(abs(dots[0]), abs(dots[1]));
	for (int i = 0; i < 2; ++i) if (max == abs(dots[i])) useAxisIndex = i;

	if (useAxisIndex == -1) return;

	// 最大の軸が負だった場合は逆ベクトルを使用
	SimpleMath::Vector2 useAxis = axes[useAxisIndex];
	if (dots[useAxisIndex] < 0) useAxis *= -1;

	// ボックスのサイズを取得
	DirectX::SimpleMath::Vector2 size = collider->GetHalfSize();

	float sizes[2] = { size.x, size.y };

	// 決まった線の中心点と端の点の3点を候補とする
	m_candidatePoints.clear();

	SimpleMath::Vector2 center = collider->GetWorldCenterPos();

	// 線中心
	SimpleMath::Vector2 planeCenter = center + useAxis * sizes[useAxisIndex];

	m_candidatePoints.push_back(planeCenter);

	// 角
	int otherAxesIndex = { (useAxisIndex + 1) % 2 };

	float offset = 0.8f;

	// 1
	m_candidatePoints.push_back(
		planeCenter +
		axes[otherAxesIndex] * sizes[otherAxesIndex] * offset
	);
	// 2
	m_candidatePoints.push_back(
		planeCenter -
		axes[otherAxesIndex] * sizes[otherAxesIndex] * offset
	);
}

void LandingCandidatePoints2D::UpdateCandidatePointsOnConvexPolygon()
{
	using namespace DirectX;

	// 凸ポリゴンコライダーに変換
	ConvexPolygonCollider2D* convex = dynamic_cast<ConvexPolygonCollider2D*>(m_ownCollider);

	// 変換に失敗したら何もしない
	if (!convex) return;

	// 向きが上方向に近い線を調べる
	std::vector<size_t> nearUpIndices;

	// ポリゴンの点を取得
	auto& vertices = convex->GetWorldVertices();

	// 全点を調べる
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		// 隣の点を繋ぐ線を形成
		SimpleMath::Vector2 line =
			vertices[(i + 1) % vertices.size()] - vertices[i];

		// 法線を生成
		SimpleMath::Vector2 normal = { -line.y, line.x };

		// 上方向ベクトルに投影して近さを調べる
		normal.Normalize();

		float upValue = SimpleMath::Vector2::UnitY.Dot(normal);

		// cosθ <= 1 / √2 を基準とする (45°以下)
		if (upValue < -CONVEX_UP_BORDER)
		{
			// 上方向に近い線として追加
			nearUpIndices.push_back(i);
		}
	}

	// 決まった線の中心点と端の点の3点を候補とする
	m_candidatePoints.clear();

	// 上方向に近い線の端点と中心点を候補点とする
	for (size_t index : nearUpIndices)
	{
		// 2点を算出
		SimpleMath::Vector2 start = vertices[index];

		size_t next = (index + 1) % vertices.size();
		SimpleMath::Vector2 end = vertices[next];

		// 自身を追加
		m_candidatePoints.push_back(start);

		// 中心点を追加
		m_candidatePoints.push_back((end + start) / 2);

		// 次の点が他の上方向に近い線の始点となるかを調べる
		bool contain = std::find(nearUpIndices.begin(), nearUpIndices.end(), next) != nearUpIndices.end();

		// ならなければ
		if (!contain)
		{
			// 次の点を候補点に追加
			m_candidatePoints.push_back(end);
		}
	}
}
