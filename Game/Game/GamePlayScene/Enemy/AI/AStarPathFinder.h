//====================================================//
// ファイル名   : AStarPathFinder.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 敵の経路をNavigationGraphとA*アルゴリズムを使って作成するクラス
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <queue>
#include <algorithm>

#include "NavigationGraphBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class AStarPathFinder
{
	// 調べるノード
	struct SearchNode
	{
		// 各コスト値
		float g = FLT_MAX;
		float h = 0;

		// 親のインデックス
		size_t parent = 0;
		size_t parentEdge = 0;

		// リストにいるかどうかのフラグ
		bool isOpen = false;
		bool isClose = false;
	};

	// オープンリストに格納する用の構造体
	struct OpenItem
	{
		size_t nodeIndex;
		float f;

		bool operator<(const OpenItem& other) const
		{
			// priority_queueは大きいものが先頭になるので逆にする
			return f > other.f;
		}
	};

private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// オープンリスト
	static std::priority_queue<OpenItem> m_openNodes;

	// ノードリスト
	static std::vector<SearchNode> m_nodes;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	AStarPathFinder() = default;
	~AStarPathFinder() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	template<typename Navigation>
	static std::vector<NavigationGraphBase::Edge> MakePath(const Navigation& nav, size_t start, size_t goal);

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 
};

//static変数の実体宣言
std::priority_queue<AStarPathFinder::OpenItem> AStarPathFinder::m_openNodes;
std::vector<AStarPathFinder::SearchNode> AStarPathFinder::m_nodes;

// 関数宣言
template<typename Navigation>
inline std::vector<NavigationGraphBase::Edge> AStarPathFinder::MakePath(const Navigation& nav, size_t start, size_t goal)
{
	// エッジのリストを取得
	const std::vector<std::vector<NavigationGraphBase::Edge>>& edges = nav.GetGraph();

	// コンポーネントリストを取得
	auto& compList = nav.GetNodes();

	// スタートかゴールが存在しなければ何もしない
	if (start >= compList.size() || goal >= compList.size()) return std::vector<NavigationGraphBase::Edge>();

	// ノードリストのリセット
	m_nodes.assign(compList.size(), SearchNode{});

	// ゴールとなるコライダーの中心座標を取得
	auto goalPos = compList[goal]->GetCenterPoint();

	// Hの値を取得するラムダ式
	auto GetHCost = [&](size_t index)->float
		{
			// 指定したインデックスの座標
			auto pos = compList[index]->GetCenterPoint();

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
		if (m_openNodes.size() == 0) return std::vector<NavigationGraphBase::Edge>();

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
			NavigationGraphBase::Edge edge = edges[current.nodeIndex][i];
		
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

	std::vector<NavigationGraphBase::Edge> way;

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
