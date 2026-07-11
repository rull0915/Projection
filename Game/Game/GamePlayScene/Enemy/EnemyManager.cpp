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

#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EnemyManager::Awake()
{
	// グラフの初期化
	m_normalNavigation.ResetGraph();

	m_normalNavigation.Initialize();

	// 2Dグラフの初期化
	m_normalNavigation2D.ResetGraph();

	m_normalNavigation2D.Initialize();

	// シーンを取得
	Scene* pScene = static_cast<GameObject*>(GetOwn())->GetScene();

	// 既に存在する全敵を取得
	auto& enemies = pScene->GetComponentRegister()->GetAllComponents<Enemy>();

	// 予約リストに追加
	for (auto& enemy : enemies)
	{
		// 一旦非アクティブにしておく
		enemy->SetActive(false);

		m_addReserves.push_back(static_cast<Enemy*>(enemy));
	}

	// 既に存在する全着地候補点を取得
	auto& pointsList = pScene->GetComponentRegister()->GetAllComponents<LandingCandidatePoints>();

	// 予約リストに追加
	for (auto& points : pointsList) m_normalNavigation.AddNode(static_cast<LandingCandidatePoints*>(points));

	// 既に存在する全2D着地候補点を取得
	auto& pointsList2D = pScene->GetComponentRegister()->GetAllComponents<LandingCandidatePoints2D>();

	// 予約リストに追加
	for (auto& points2D : pointsList2D) m_normalNavigation2D.AddNode(static_cast<LandingCandidatePoints2D*>(points2D));

	// スタート時イベントを追加する
	m_eventToken =
	EventBus<GamePlayEvent>::Register(
		GamePlayEvent::Start,
		[&]() {
			m_isStarted = true;
			for (auto& enemy : m_enemies) enemy->SetActive(true);
			for (auto& enemy : m_addReserves) enemy->SetActive(true);
		}
	);
}

void EnemyManager::Start()
{
	// フラグのリセット
	m_is2D = false;

	// プレイヤーを取得
	GameObject* player = static_cast<GameObject*>(GetOwn())->GetScene()->GetObjectFinder()->FindWithNameInWorld("Player");

	if (player) m_playerTransform = player->GetComponent<Transform>();
}

void EnemyManager::OnDestroy()
{
	// イベント削除
	EventBus<GamePlayEvent>::Remove(m_eventToken);
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
		if (m_is2D)
		{
			m_normalNavigation2D.Update();
		}

		else
		{
			m_normalNavigation.Update();
		}

		// タイマーをリセット
		m_nowTime = 0;
	}

	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 playerPos = m_playerTransform->GetWorldPosition();

	// プレイヤーが一定以上動いたら敵の道を再計算する
	if ((playerPos - m_oldPlayerPosition).LengthSquared() >= WAY_UPDATE_BORDER)
	{
		// 2次元の時
		if (m_is2D)
		{
			// プレイヤーの候補点
			LandingCandidatePoints2D* playerPoints = nullptr;

			// プレイヤーコンポーネントがあれば
			if (auto pl = m_playerTransform->GetOwn()->GetComponent<Player>())
			{
				playerPoints = pl->GetLandingPoints2D();
			}

			// プレイヤーがない or まだ未着地ならスキップ
			if (!playerPoints) return;

			// プレイヤーのインデックスを取得
			size_t playerIndex = m_normalNavigation2D.GetIndex(playerPoints);

			// 登録されていなければスキップ
			if (playerIndex == -1) return;

			// ノードを取得
			auto& nodes = m_normalNavigation2D.GetNodes();

			// 全ての敵をチェック
			for (auto& enemy : m_enemies)
			{
				if (!enemy->IsActive()) continue;

				// 敵の候補点のインデックスを取得
				size_t enemyIndex = m_normalNavigation2D.GetIndex(enemy->GetLandingPoints2D());

				// 登録されていなければスキップ
				if (enemyIndex == -1) continue;

				// 道を再計算
				auto edges = AStarPathFinder::MakePath(m_normalNavigation2D, enemyIndex, playerIndex);
				m_debugPath = edges;

				// 座標に変換
				std::vector<PathFollower::Path2D> way(edges.size());

				// 全辺をループ
				for (size_t i = 0; i < edges.size(); ++i)
				{
					// 候補点を取得
					auto goalPoints = nodes[edges[i].goalIndex]->GetPoints();
					auto startPoints = nodes[edges[i].ownIndex]->GetPoints();

					// 範囲外チェック
					if (edges[i].goalPoint >= goalPoints.size() || edges[i].startPoint >= startPoints.size()) continue;

					// ゴールの座表
					DirectX::SimpleMath::Vector2 goal = goalPoints[edges[i].goalPoint];

					// スタートの座標
					DirectX::SimpleMath::Vector2 start = startPoints[edges[i].startPoint];

					// パスに追加
					way[i] = { start, goal, edges[i].cost };
				}

				// 敵に通知
				enemy->SetWay(way);
			}
		}
		else
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
				// 敵の候補点のインデックスを取得
				size_t enemyIndex = m_normalNavigation.GetIndex(enemy->GetLandingPoints());

				// 登録されていなければスキップ
				if (enemyIndex == -1) continue;

				// 道を再計算
				auto edges = AStarPathFinder::MakePath(m_normalNavigation, enemyIndex, playerIndex);
				m_debugPath = edges;

				// 座標に変換
				std::vector<PathFollower::Path> way(edges.size());

				// 全辺をループ
				for (size_t i = 0; i < edges.size(); ++i)
				{
					// 候補点を取得
					auto goalPoints = nodes[edges[i].goalIndex]->GetPoints();
					auto startPoints = nodes[edges[i].ownIndex]->GetPoints();

					// 範囲外チェック
					if (edges[i].goalPoint >= goalPoints.size() || edges[i].startPoint >= startPoints.size()) continue;

					// ゴールの座表
					DirectX::SimpleMath::Vector3 goal = goalPoints[edges[i].goalPoint];

					// スタートの座標
					DirectX::SimpleMath::Vector3 start = startPoints[edges[i].startPoint];

					// パスに追加
					way[i] = { start, goal, edges[i].cost };
				}

				// 敵に通知
				enemy->SetWay(way);
			}
		}
	}
}

void EnemyManager::DebugRenderer(Renderer& renderer)
{
	if (m_is2D) m_normalNavigation2D.DebugDraw(m_debugPath, renderer);
	else m_normalNavigation.DebugDraw(m_debugPath, renderer);
}

void EnemyManager::ChangeDimantion()
{
	// 自身のフラグを変更
	m_is2D = !m_is2D;

	// 管理している全ての敵を変更
	for (auto& enemy : m_enemies)
	{
		// 次元を変更
		enemy->SetIs2D(m_is2D);
	}

	if (m_is2D)
	{
		// 2Dグラフの初期化
		m_normalNavigation2D.Initialize();
	}
}
