//====================================================//
// ファイル名  : TestPlayScene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/07
//
// 概要       : TestPlaySceneシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TestPlayScene.h"

#include "Game/Game.h"
#include "Editor/Loader/ObjectLoader.h"

#include "Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
TestPlayScene::TestPlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
{
}

TestPlayScene::~TestPlayScene()
{
}

// 初期化関数
void TestPlayScene::Initialize()
{
	// テストファイルの読み込み
	ObjectLoader::LoadSceneFromFile(L"Resources/Scenes/TestPlayScene.scene", this);
}

// 更新関数
void TestPlayScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	// エスケープキーでエディットへ戻る
	if (Input::Key::Get(Input::State::Down, Input::Key::Code::Escape))
	{
		// シーン読み込みなおし
		ChangeScene("Edit");
	}
}

// 描画関数
void TestPlayScene::Render(Renderer& renderer)
{
}

// 終了関数
void TestPlayScene::Finalize()
{
}
