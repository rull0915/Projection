//====================================================//
// ファイル名  : EnemyManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要       : 敵管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EnemyManager.h"

#include "AI/AStarPathFinder.h"
#include "../Player/Player.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EnemyManager::Initialize()
{
	// グラフの初期化
	m_normalNavigation.ResetGraph();

	m_normalNavigation.Initialize();
}

void EnemyManager::Update(const GameTimer& timer)
{
	// 時間を加算
	m_nowTime += timer.GetElapsedTime();

	// 予約の反映
	AddReserved();
	RemoveReserved();

	// 更新間隔を超えたら
	if (m_nowTime >= GRAPH_UPDATE_DISTANCE)
	{
		// グラフを更新
		m_normalNavigation.Update();

		// タイマーをリセット
		m_nowTime = 0;
	}

	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 playerPos = m_playerTransform->GetWorldPosition();

	// プレイヤーが一定以上動いたら敵の道を再計算する
	if ((playerPos - m_oldPlayerPosition).LengthSquared() >= WAY_UPDATE_BORDER)
	{
		// プレイヤーの候補点
		LandingCandidatePoints* playerPoints = nullptr;

		// プレイヤーコンポーネントがあれば
		if (auto pl = m_playerTransform->GetOwn()->GetComponent<Player>())
		{
			playerPoints = pl->GetLandingPoints();
		}

		// プレイヤーがない or まだ未着地ならスキップ
		if (!playerPoints) return;

		// プレイヤーのインデックスを取得
		size_t playerIndex = m_normalNavigation.GetIndex(playerPoints);
		
		// 登録されていなければスキップ
		if (playerIndex == -1) return;

		// ノードを取得
		auto& nodes = m_normalNavigation.GetNodes();

		// 全ての敵をチェック
		for (auto& enemy : m_enemies)
		{
			// アイドル状態でなければ何もしない
			// if (enemy->GetNowState() != EnemyStateID::Idle) continue;

			// 敵の候補点のインデックスを取得
			size_t enemyIndex = m_normalNavigation.GetIndex(enemy->GetLandingPoints());
		
			// 登録されていなければスキップ
			if (enemyIndex == -1) continue;

			// 道を再計算
			auto edges = AStarPathFinder::MakePath(m_normalNavigation, enemyIndex, playerIndex);

			// 座標に変換
			std::vector<PathFollower::Path> way(edges.size());

			// 全辺をループ
			for (size_t i = 0; i < edges.size(); ++i)
			{
				// ゴールの座表
				DirectX::SimpleMath::Vector3 goal = nodes[edges[i].goalIndex]->GetPoints()[edges[i].goalPoint];

				// スタートの座標
				DirectX::SimpleMath::Vector3 start = nodes[edges[i].ownIndex]->GetPoints()[edges[i].startPoint];

				// パスに追加
				way[i] = { start, goal, edges[i].cost };
			}

			// 敵に通知
			enemy->SetWay(way);
		}
	}
}
