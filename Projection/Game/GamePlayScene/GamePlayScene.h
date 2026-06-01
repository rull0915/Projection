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

#include "GameLib/GameMath/Easing.h"

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

	Canvas* m_pTestCanvas;
	UIObject* m_pTestUI;

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
    void Update(float elapsedTime) override;
    void Render(Renderer& renderer) override;
    void Finalize() override;

    void Retry()
    {
        BaseFinalize();
        BaseInitialize();
    }

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

    void GenerateCube(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale = { 1, 1, 1 });
};

// 移動テスト関数
static DirectX::SimpleMath::Vector3 MoveMotion(float ratio)
{
	//// 半径		
 //   float radius = 10.0f;

 //   // 0~1で一周
	//float angle = PI_F * 2 * ratio;
	//return DirectX::SimpleMath::Vector3(radius * std::cos(angle), radius * std::sin(angle), 1);

    DirectX::SimpleMath::Vector3 start = { 0, 0, 0 }, end = { 50, 0, 0 };

    float newRatio = Easing::EaseInOutQuad(ratio);
    return DirectX::SimpleMath::Vector3::Lerp(start, end, newRatio);
}
