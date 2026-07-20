//====================================================//
// ファイル名  : NavigationGraph.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "NavigationGraph.h"

#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void NavigationGraph::Initialize()
{
	// 初期状態でグラフを生成する
	InitializeGraph();
}

void NavigationGraph::Update()
{
	// 予約済みの追加
	AddReserved();

	// 予約済みの削除
	RemoveReserved();

	// グラフの更新をする
	UpdateGraph();
}

void NavigationGraph::DebugDraw(REngine::Renderer& renderer)
{
	// 繋がっているコライダー同士を結ぶ線を描く
	for (auto& edges : GetGraph())
	{
		DebugDraw(edges, renderer);
	}
}

void NavigationGraph::DebugDraw(const std::vector<Edge>& edges, REngine::Renderer& renderer, DirectX::SimpleMath::Color color)
{
	for (auto& edge : edges)
	{
		// 存在していないnodeを指していたらスキップ
		if (edge.ownIndex >= m_nodes.size() || edge.goalIndex >= m_nodes.size()) continue;

		if (edge.startPoint >= m_nodes[edge.ownIndex]->GetPoints().size() || edge.goalPoint >= m_nodes[edge.goalIndex]->GetPoints().size()) continue;

		DirectX::SimpleMath::Vector3 points[2] =
		{
			m_nodes[edge.ownIndex]->GetPoints()[edge.startPoint],
			m_nodes[edge.goalIndex]->GetPoints()[edge.goalPoint]
		};

		//// 繋ぐ線を描画
		//renderer.Draw().Line(points[0], points[1], color);
	}
}

bool NavigationGraph::CanJump(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 target, float& time)
{
	// Y方向の距離の差を求める
	float yDistance = target.y - start.y;

	// 最大より大きければfalse
	if (yDistance > GetMaxHeight()) return false;

	// 初速を求める
	float initV = GetJumpImpluse() / GetEnemyMass();

	// 到達できる時間tを求める

	// 判別式Dの作成
	float D = initV * initV - 2 * GetGravity() * yDistance;

	// 解となるtがなければfalse
	if (D < 0) return false;

	// 値が大きい方の解をtとする
	float t = (initV + std::sqrtf(D)) / GetGravity();

	// 求めたtまでに水平方向を移動しきれるかを調べる
	DirectX::SimpleMath::Vector3 horizontalVec = { target - start };
	horizontalVec.y = 0;

	// 水平方向の距離
	float horizontalDistanceSq = horizontalVec.LengthSquared();

	float maxDistance = GetHorizontalVelocity() * t;

	// 最高速度で移動し続けても届かないならfalse
	if (maxDistance * maxDistance < horizontalDistanceSq) return false;

	time = t;

	// 水平垂直両方満たせばジャンプで届く
	return true;
}

void NavigationGraph::InitializeGraph()
{
	// グラフのリセット
	GetGraph().clear();

	// 再構築
	GetGraph().resize(m_nodes.size());

	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		for (size_t j = 0; j < m_nodes.size(); ++j)
		{
			// 同じノードならスキップ
			if (i == j) continue;

			// ノードがつながるか調べる
			BuildConnection(i, j);
		}
	}
}

void NavigationGraph::UpdateGraph()
{
	// removeで使用するラムダ式
	auto lambda = [&](const Edge& edge) -> bool 
		{
			return m_nodes[edge.ownIndex]->IsChanged() || m_nodes[edge.goalIndex]->IsChanged();
		};

	// 条件を満たしたすべてのエッジを削除する
	for (auto& edges : GetGraph())
	{
		std::erase_if(edges, lambda);
	}

	// 変更されているノードからエッジを再構築する
	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		// 変更されていなければスキップ
		if (!m_nodes[i]->IsChanged()) continue;

		// 他の全てのノードに対して繋がりを調べる
		for (size_t j = 0; j < m_nodes.size(); ++j)
		{
			// 自分と同じノードはスキップ
			if (i == j) continue;

			// 調べる
			BuildConnection(i, j);
			BuildConnection(j, i);
		}
		// フラグのリセット
		m_nodes[i]->ResetChangedFlag();
	}
}

void NavigationGraph::BuildConnection(size_t first, size_t second)
{
	const std::vector<DirectX::SimpleMath::Vector3>& firstPoints = m_nodes[first]->GetPoints();
	const std::vector<DirectX::SimpleMath::Vector3>& secondPoints =m_nodes[second]->GetPoints();
		
	// 届くかつ最も距離の近い候補点のペアを調べる
	float min = FLT_MAX, minT = 0;
	std::pair<size_t, size_t> pair = { 0, 0 };

	for (size_t i = 0; i < firstPoints.size(); ++i)
	{
		for (size_t j = 0; j < secondPoints.size(); ++j)
		{
			float t = 0;

			// ジャンプで届くなら
			if (CanJump(firstPoints[i], secondPoints[j], t))
			{
				// 長さを調べる
				float lenSq = (secondPoints[j] - firstPoints[i]).LengthSquared();

				// 最小値が更新されたら
				if (lenSq < min)
				{
					min = lenSq;

					pair = { i, j };

					minT = t;
				}
			}
		}
	}

	// 1つでも存在していれば
	if (min != FLT_MAX)
	{
		// 辺を追加
		GetGraph()[first].push_back
		(
			{ first, second, pair.first, pair.second, minT }
		);
	}
}
