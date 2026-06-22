//====================================================//
// ファイル名   : GamePlayScene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/05
//
// 概要 : ゲームプレイシーン
//
// 更新履歴 : 
// 2026/04/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/Scene/Scene.h"

#include "ChangeDimention/DimentionManager.h"
#include "Enemy/AI/NavigationGraph.h"

//====================================================//
// 前方宣言
//====================================================//
class Game;

//====================================================//
// クラス宣言
//====================================================//
class GamePlayScene : public Scene
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    Game* m_pGame;

    GameObject* m_camera;
    GameObject* m_player;

    DimentionManager m_dimentionManager;

    NavigationGraph m_testNav;

	Canvas* m_pTestCanvas;
	UIObject* m_pTestUI;

    size_t m_startIndex, m_goalIndex;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    GamePlayScene(Game* pGame);
    ~GamePlayScene();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Initialize() override;
    void Update(const GameTimer& gameTimer) override;
    void Render(Renderer& renderer) override;
    void Finalize() override;

    void RegisterComponentOnDerived(BaseComponent* component) override;
    void UnRegisterComponentOnDerived(BaseComponent* component) override;

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

    void InitializeUITest();

    GameObject* GenerateCube(DirectX::SimpleMath::Vector3 position, int type, DirectX::SimpleMath::Vector3 scale = { 1, 1, 1 }, DirectX::SimpleMath::Vector3 rot = { 0, 0, 0 });

    // 次元変更を行う関数
    void TryChangeDimention();

};

static DirectX::SimpleMath::Vector3 TestMove(float time)
{
    return { sinf(time) * 10, 0, 0};
}