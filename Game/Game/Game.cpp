//====================================================//
// ファイル名  : Game.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/02
//
// 概要       : ゲームクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"
#include "Game.h"

#include "Scene/SceneManager.h"
#include "GameInitializer.h"

void Game::OnInitialize()
{
	// ゲーム部分の初期化
	GameInitializer::Initialize();

	// ゲームの初期化
	GameInitializer::Initialize();

	// ====== シーンの登録 ====== //
	REngine::SceneManager::Instance().RegisterScene("Title", L"Resources/Scenes/TitleScene.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage1", L"Resources/Scenes/Stage1.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage2", L"Resources/Scenes/Stage2.scene");
	REngine::SceneManager::Instance().RegisterScene("Stage3", L"Resources/Scenes/Stage3.scene");
	REngine::SceneManager::Instance().RegisterScene("Select", L"Resources/Scenes/SelectScene.scene");
	REngine::SceneManager::Instance().RegisterScene("Clear", L"Resources/Scenes/ClearScene.scene");
	REngine::SceneManager::Instance().RegisterScene("GameOver", L"Resources/Scenes/GameOverScene.scene");

	REngine::SceneManager::Instance().RegisterScene("GUITest", L"Resources/Scenes/GUITest.scene");

	// 開始時のシーンを設定
	REngine::SceneManager::Instance().SetStartScene("Title");
}
