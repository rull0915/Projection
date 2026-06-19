//====================================================//
// ファイル名  : AStarPathFinder.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要       : 敵の経路をNavigationGraphとA*アルゴリズムを使って作成するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "AStarPathFinder.h"

#include <algorithm>

//====================================================//
// static変数の実体宣言
//====================================================//

// オープンリスト
std::priority_queue<AStarPathFinder::OpenItem> AStarPathFinder::m_openNodes;

// ノードリスト
std::vector<AStarPathFinder::SearchNode> AStarPathFinder::m_nodes;

//====================================================//
// 関数の実体宣言
//====================================================//

std::vector<NavigationGraph::Edge> AStarPathFinder::MakePath(const NavigationGraph& nav, size_t start, size_t goal)
{
	// エッジのリストを取得
	const std::vector<std::vector<NavigationGraph::Edge>>& edges = nav.GetGraph();

	// ノードリストを再構築
	m_nodes.clear();
	// コンポーネントリストを取得
	auto& compList = nav.GetNodes();

	// スタートかゴールが存在しなければ何もしない
	if (start < 0 || start >= compList.size() || goal < 0 || goal >= compList.size()) return std::vector<NavigationGraph::Edge>();

	m_nodes.resize(compList.size());

	// ゴールとなるコライダーの中心座標を取得
	DirectX::SimpleMath::Vector3 goalPos = compList[goal]->GetCenterPoint();

	// Hの値を取得するラムダ式
	auto GetHCost = [&](size_t index)->float
		{
			// 指定したインデックスの座標
			DirectX::SimpleMath::Vector3 pos = compList[index]->GetCenterPoint();

			// ゴールとの長さの二乗を返す
			return (goalPos - pos).LengthSquared();
		};

	// スタートノードの初期化
	m_nodes[start].g = 0;
	m_nodes[start].h = GetHCost(start);

	// オープンリストの初期化
	m_openNodes = std::priority_queue<OpenItem>();

	// スタートノードをオープンリストに追加
	m_openNodes.push({ start, m_nodes[start].h });

	m_nodes[start].isOpen = true;

	while (true)
	{
		// オープンリストが空になったらゴール不可
		if (m_openNodes.size() == 0) return std::vector<NavigationGraph::Edge>();

		// オープンリストから最もf値の低いノードを取得
		OpenItem current = m_openNodes.top();

		// オープンリストから削除
		m_openNodes.pop();
		m_nodes[current.nodeIndex].isOpen = false;

		// クローズにする
		if (m_nodes[current.nodeIndex].isClose) continue;
		m_nodes[current.nodeIndex].isClose = true;

		// ゴールであればループ終了
		if (current.nodeIndex == goal) break;

		// 隣接ノードを調べる
		for (size_t i = 0; i < edges[current.nodeIndex].size(); ++i)
		{
			// 調べるエッジを取得
			NavigationGraph::Edge edge = edges[current.nodeIndex][i];
		
			size_t targetIndex = edge.goalIndex;

			// クローズならスキップ
			if (m_nodes[targetIndex].isClose) continue;

			float ng = m_nodes[current.nodeIndex].g + (edge.cost);

			// 最小値を更新したら
			if (!m_nodes[targetIndex].isOpen || ng < m_nodes[targetIndex].g) {

				// gを更新
				m_nodes[targetIndex].g = ng;

				float nh = GetHCost(targetIndex);

				// hを更新
				m_nodes[targetIndex].h = nh;

				// 親を自身に設定
				m_nodes[targetIndex].parent = { current.nodeIndex };
				m_nodes[targetIndex].parentEdge = i;

				// オープンリストに追加
				m_nodes[targetIndex].isOpen = true;

				m_openNodes.push({ targetIndex, ng + nh });
			}
		}
	}

	// ゴールから逆に辿る
	size_t current = goal;

	std::vector<NavigationGraph::Edge> way;

	// 親がいなくなるまでループ
	while (current != start)
	{
		// 親のインデックスを取得
		size_t parent = m_nodes[current].parent;

		// 道に追加
		way.push_back(edges[parent][m_nodes[current].parentEdge]);

		// 今の位置を更新
		current = m_nodes[current].parent;
	}

	// 逆順に入れ替え
	std::reverse(way.begin(), way.end());

	return way;
}
