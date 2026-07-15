//====================================================//
// ファイル名   : NavigationGraph.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要 : 敵が移動できる有向グラフを作成するクラス
//
// 更新履歴 :
// 2026/06/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include "../Components/LandingCandidatePoints.h"
#include "NavigationGraphBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class NavigationGraph : public NavigationGraphBase
{
public:

private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 扱うノードの配列
	std::vector<LandingCandidatePoints*> m_nodes;

	// 追加予約中のノード
	std::vector<LandingCandidatePoints*> m_addReserves;
	// 削除予約中のノード
	std::vector<LandingCandidatePoints*> m_removeReserves;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	NavigationGraph(float jumpImpulse, float mass, float horizontalVelocity)
		: NavigationGraphBase(jumpImpulse, mass, horizontalVelocity)
		, m_nodes{}
		, m_addReserves{}
		, m_removeReserves{}
	{}

	~NavigationGraph() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// デバッグ描画
	void DebugDraw(REngine::Renderer& renderer) override;

	void DebugDraw(const std::vector<Edge>& edges, REngine::Renderer& renderer, DirectX::SimpleMath::Color color = { 0, 1, 0, 0 }) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ノードを追加する関数
	void AddNode(LandingCandidatePoints* node)
	{
		m_addReserves.push_back(node);
	}

	// ノードを削除する関数
	void RemoveNode(LandingCandidatePoints* node)
	{
		m_removeReserves.push_back(node);
	}

	// インデックスを取得する関数
	int GetIndex(LandingCandidatePoints* point)
	{
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			if (m_nodes[i] == point) return i;
		}

		return -1;
	}

	// 管理しているコンポーネントを取得する関数
	const std::vector<LandingCandidatePoints*> GetNodes() const { return m_nodes; }

	void ResetGraph()
	{
		m_addReserves.clear();
		m_removeReserves.clear();
		m_nodes.clear();

		NavigationGraphBase::ResetGraph();
	}
	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// ジャンプで到達できるか調べる関数
	bool CanJump(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 target, float& time);

	// グラフを再構築する関数
	void InitializeGraph() override;

	// グラフを更新する関数
	void UpdateGraph() override;

	// 候補点同士
	void BuildConnection(size_t first, size_t second) override;

	// 追加予約済みノードを追加する関数
	void AddReserved()
	{
		// 予約がなければ何もしない
		if (m_addReserves.size() == 0) return;

		// 全ての追加予約を調べる
		for (auto& node : m_addReserves)
		{
			// リストに追加
			m_nodes.push_back(node);
		}

		// リストを初期化
		m_addReserves.clear();

		// グラフを再構築
		InitializeGraph();
	}

	void RemoveReserved()
	{
		// 予約がなければ何もしない
		if (m_removeReserves.size() == 0) return;

		// 全ての追加予約を調べる
		for (auto& node : m_removeReserves)
		{
			// 条件を満たすノードを削除
			std::erase_if(m_nodes, [&](const LandingCandidatePoints* n) -> bool { return n == node; });
		}

		// リストを初期化
		m_removeReserves.clear();

		// グラフを再構築
		InitializeGraph();
	}

};
