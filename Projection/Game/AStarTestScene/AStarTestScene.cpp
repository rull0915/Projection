#include "pch.h"

#include <stack>

#include "AStarTestScene.h"
#include "../Game.h"

// 入力
#include "GameLib/Input/KeyInput.h"

#include "GameLib/Random.h"

// コンストラクタ
AStarTestScene::AStarTestScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_goal{ WIDTH - 6, HEIGHT - 6 }
	, m_start{ 5, 5 }
	, m_nodes{}
	, m_open{}
{
}

AStarTestScene::~AStarTestScene()
{
}

// 初期化関数
void AStarTestScene::Initialize()
{
	m_nodes.clear();
	m_mazeStage.clear();

	m_way.clear();
	m_open = std::priority_queue<AItem>();

	// 迷路作成
	ResetMaze();

	MakeMaze();

	m_nodes.resize(HEIGHT);
	for (auto& col : m_nodes) col.resize(WIDTH);

	m_nodes[m_start.y][m_start.x].g = 0;
	m_nodes[m_start.y][m_start.x].h = GetHCost(m_start.x, m_start.y);
	m_nodes[m_start.y][m_start.x].isOpen = true;

	m_open.push({ m_start.x, m_start.y, GetHCost(m_start.x, m_start.y) });
}

// 更新関数
void AStarTestScene::Update(const GameTimer& gameTimer)
{
	static bool isGoaled = false;

	if (!isGoaled)// && KeyInput::GetKeyDown(KeyCode::Space))
	{
		int id = AStep();
		int count = 0;

		while (id == 0 && count < 100)
		{
			id = AStep();

			count++;
		}

		if (id == 1)
		{
			isGoaled = true;

			APos current = m_goal;

			// 親がいなくなるまでループ
			while (!(current.x == m_start.x && current.y == m_start.y))
			{
				// 道に追加
				m_way.push_back(current);

				// 今の位置を更新
				current = m_nodes[current.y][current.x].parent;
			}
		}
		else if (id == -1)
		{
			isGoaled = true;
		}
	}

	if (KeyInput::GetKeyDown(KeyCode::R))
	{
		isGoaled = false;

		Finalize();
		Initialize();
	}
}

// 描画関数
void AStarTestScene::Render(Renderer& renderer)
{
	MapRenderer(renderer);
}

// 終了関数
void AStarTestScene::Finalize()
{
}

void AStarTestScene::MapRenderer(Renderer& renderer)
{
	static const DirectX::SimpleMath::Vector2 offset = { 10, 10 };
	static const DirectX::SimpleMath::Vector2 size = { 4, 4 };

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			DirectX::SimpleMath::Vector2 min = offset + DirectX::SimpleMath::Vector2{ size.x * j, size.y * i };
			DirectX::SimpleMath::Vector2 max = min + size;

			int color = 0xFFFFFF;

			if (i == m_goal.y && j == m_goal.x) color = 0xCC0000;
			else if (i == m_start.y && j == m_start.x) color = 0xCCCC00;
			else if (m_mazeStage[i][j] == 1) color = 0x222222;
			else if (ContainWay(j, i)) color = 0x3333DD;
			else if (m_nodes[i][j].isOpen) color = 0xAAAAAA;
			else if (m_nodes[i][j].isClose) color = 0x999999;

			renderer.Draw().UI().Box(min, max, color);
		}
	}
}

int AStarTestScene::AStep()
{
	// オープンリストが空になったらゴール不可
	if (m_open.size() == 0) return -1;

	// オープンリストから最もfの低いノードを取得
	AItem current = m_open.top(); 

	m_open.pop();

	// クローズにする
	if(m_nodes[current.y][current.x].isClose) return 0;

	m_nodes[current.y][current.x].isOpen = false;
	m_nodes[current.y][current.x].isClose = true;

	// ゴールであれば
	if (current.x == m_goal.x && current.y == m_goal.y) return 1;

	// 周囲のマスを調べる
	static constexpr int xPoses[8] = { 0, 1, 0, -1, -1, 1, 1, -1 };
	static constexpr int yPoses[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };

	for (int i = 0; i < 4; i++)
	{
		// 調べる座標を確定
		int nx = current.x + xPoses[i], ny = current.y + yPoses[i];

		// ステージ外ならスキップ
		if (nx < 0 || ny < 0 || nx >= m_nodes[0].size() || ny >= m_nodes.size()) continue;

		// 壁ならスキップ
		if (m_mazeStage[ny][nx] == 1) continue;

		// クローズならスキップ
		if (m_nodes[ny][nx].isClose) continue;

		float ng = m_nodes[current.y][current.x].g + (i < 4 ? 1.0f : 1.414f);

		// 最小値を更新したら
		if (!m_nodes[ny][nx].isOpen || ng < m_nodes[ny][nx].g) {

			// gを更新
			m_nodes[ny][nx].g = ng;

			// hを更新
			m_nodes[ny][nx].h = GetHCost(nx, ny);

			// 親を自身に設定
			m_nodes[ny][nx].parent = { current.x, current.y };

			// オープンリストに追加
			m_nodes[ny][nx].isOpen = true;

			m_open.push({ nx, ny, ng + m_nodes[ny][nx].h });
		}
	}

	return 0;
}

void AStarTestScene::MakeMaze()
{
	std::stack<APos> stack;

	// 穴を掘る開始地点
	APos start{ 1, 1 };

	// 開始地点を道にする
	m_mazeStage[start.y][start.x] = 0;

	stack.push(start);

	APos goal;

	size_t max = 0;

	while (!stack.empty())
	{
		// 穴を掘るやつの現在の位置をスタックの一番上からもらう
		APos current = stack.top();

		// 行ける方向もらう
		auto neighbors = GetUnvisitedNeighbors(current);
		
		if (!neighbors.empty())
		{
			// 次に掘る位置をランダムに決める
			APos next = neighbors[Random::Get(0, neighbors.size() - 1)];

			// 次の位置を道にする
			m_mazeStage[next.y][next.x] = 0;
			int wallX = (current.x + next.x) / 2;
			int wallY = (current.y + next.y) / 2;

			// 今の位置と次の位置の間を道にする
			m_mazeStage[wallY][wallX] = 0;
			
			// 掘った位置をスタックに渡す（現在位置をここで更新）
			stack.push(next);

			// ゴール
			if (stack.size() > max)
			{
				max = stack.size();
				goal = stack.top();
			}
		}
		else
		{
			// 掘る位置がないなら戻る
			stack.pop();
		}
	}

	// スタートとゴールは空白にする
	m_mazeStage[m_start.y][m_start.x] = 0;
	m_mazeStage[m_goal.y][m_goal.x] = 0;
}

// 現在位置から上下左右の2マス先を調べ、未訪問セル（UNVISITED）の座標をすべて返す
std::vector<APos> AStarTestScene::GetUnvisitedNeighbors(APos pos)
{
	std::vector<APos> unvisitedPoints;
	int nx = 0;
	int ny = 0;

	// 上に掘る
	if (IsValidIndex(pos.x, pos.y - 2) && m_mazeStage[pos.y - 2][pos.x] == -1)
	{
		ny = pos.y - 2;
		nx = pos.x;
		// 未訪問の位置を保管する
		unvisitedPoints.push_back(APos{ nx, ny });
	}

	// 右に掘る
	if (IsValidIndex(pos.x + 2, pos.y) && m_mazeStage[pos.y][pos.x + 2] == -1)
	{
		ny = pos.y;
		nx = pos.x + 2;
		unvisitedPoints.push_back(APos{ nx, ny });
	}

	// 下に掘る
	if (IsValidIndex(pos.x, pos.y + 2) && m_mazeStage[pos.y + 2][pos.x] == -1)
	{
		ny = pos.y + 2;
		nx = pos.x;
		unvisitedPoints.push_back(APos{ nx, ny });
	}

	// 左に掘る
	if (IsValidIndex(pos.x - 2, pos.y) && m_mazeStage[pos.y][pos.x - 2] == -1)
	{
		ny = pos.y;
		nx = pos.x - 2;
		unvisitedPoints.push_back(APos{ nx, ny });
	}

	return unvisitedPoints;
}

void AStarTestScene::ResetMaze()
{
	m_mazeStage.resize(HEIGHT);

	for (int i = 0; i < HEIGHT; i++)
	{
		m_mazeStage[i].resize(WIDTH);

		for (int j = 0; j < WIDTH; j++)
		{
			m_mazeStage[i][j] = 1;
			if ((i % 2 == 1) && (j % 2 == 1))// && rand() % 10 < 8)
			{
				m_mazeStage[i][j] = -1;
			}
		}
	}
}
