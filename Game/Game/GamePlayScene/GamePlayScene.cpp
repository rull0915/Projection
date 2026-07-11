#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

// 管理クラス
#include "System/ResourceManager.h"

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_renderTarget{ std::make_unique<RenderTarget>() }
	, m_birdCamera{ nullptr }
{
}

GamePlayScene::~GamePlayScene()
{
}

// 初期化関数
void GamePlayScene::Initialize()
{
	// 初期化
	m_renderTarget->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		1280, 720
	);
}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	// 警告潰し
	gameTimer;
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
}

void GamePlayScene::RenderOnScreen(Renderer& renderer)
{
}

// 終了関数
void GamePlayScene::Finalize()
{
}
