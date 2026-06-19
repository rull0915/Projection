//====================================================//
// ファイル名   : AStarTestScene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : A*アルゴリズムを練習するシーン
//
// 更新履歴 : 
// 2026/04/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/Scene/Scene.h"
#include <queue>

//====================================================//
// 前方宣言
//====================================================//
class Game;

//====================================================//
// 構造体宣言
//====================================================//

struct APos
{
    int x, y;

    bool operator==(const APos& other)
    {
        return x == other.x && y == other.y;
    }
};

struct ANode
{
    float g = FLT_MAX, h = 0;

    APos parent{ -1, -1 };

    bool isOpen = false, isClose = false;
};


struct AItem
{
    int x, y;
    float f;

    bool operator< (const AItem& o) const { return f > o.f; }
};

//====================================================//
// クラス宣言
//====================================================//
class AStarTestScene : public Scene
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------
    // 迷路ステージ
    std::vector<std::vector<int>> m_mazeStage;

    static constexpr int WIDTH = 151;
    static constexpr int HEIGHT = 151;

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    Game* m_pGame;

    std::vector<std::vector<ANode>> m_nodes;

    std::priority_queue<AItem> m_open;

    std::vector<APos> m_way;

    APos m_start, m_goal;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    AStarTestScene(Game* pGame);
    ~AStarTestScene();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Initialize() override;
    void Update(const GameTimer& gameTimer) override;
    void Render(Renderer& renderer) override;
    void Finalize() override;

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------
    void MapRenderer(Renderer& renderer);

    int AStep();

    void MakeMaze();

    float GetHCost(int x, int y)
    {
        float dX = x - m_goal.x, dY = y - m_goal.y;

        //return 0;
        //return abs(dX) + abs(dY);
        return dX * dX + dY * dY;
    }

    bool ContainWay(int x, int y)
    {
        auto it = std::find(m_way.begin(), m_way.end(), APos{ x, y });
        return it != m_way.end();
    }

    std::vector<APos> GetUnvisitedNeighbors(APos pos);

    // 範囲チェック
    bool IsValidIndex(int x, int y)
    {
        return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
    }

    void ResetMaze();
};
