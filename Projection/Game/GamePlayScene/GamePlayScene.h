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
#include "Scene/Scene.h"

#include "ChangeDimention/DimentionManager.h"
#include "Enemy/EnemyManager.h"

//====================================================//
// 前方宣言
//====================================================//
class Game;

enum class Test
{
	A,
};

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

	// カメラ
	GameObject* m_camera;

	// プレイヤー
	GameObject* m_player;

	// 次元管理クラス
	DimentionManager m_dimentionManager;

	// 敵管理クラス
	EnemyManager m_enemyManager;

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

	void RegisterComponentOnDerived(ComponentBase* component) override;
	void UnRegisterComponentOnDerived(ComponentBase* component) override;

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

	// 次元変更を行う関数
	void TryChangeDimention();

};

static DirectX::SimpleMath::Vector3 TestMove(float time)
{
	return { sinf(time) * 10, 0, 0 };
}