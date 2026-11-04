/**
 * @file    Game.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/02
 */

#include "pch.h"
#include "Game.h"

#include "Scene/SceneManager.h"
#include "GameInitializer.h"

void Game::OnInitialize()
{
	// ゲーム部分の初期化
	GameInitializer::Initialize();
	
	// ====== シーンの登録 ====== //
	RHEngine::SceneManager::Instance().RegisterScene("Title", L"Resources/Scenes/TitleScene.scene");
	RHEngine::SceneManager::Instance().RegisterScene("Stage1", L"Resources/Scenes/Stage1.scene");
	RHEngine::SceneManager::Instance().RegisterScene("Stage2", L"Resources/Scenes/Stage2.scene");
	RHEngine::SceneManager::Instance().RegisterScene("Stage3", L"Resources/Scenes/Stage3.scene");
	RHEngine::SceneManager::Instance().RegisterScene("Select", L"Resources/Scenes/SelectScene.scene");
	RHEngine::SceneManager::Instance().RegisterScene("Clear", L"Resources/Scenes/ClearScene.scene");
	RHEngine::SceneManager::Instance().RegisterScene("GameOver", L"Resources/Scenes/GameOverScene.scene");

	RHEngine::SceneManager::Instance().RegisterScene("GUITest", L"Resources/Scenes/GUITest.scene");

	// 開始時のシーンを設定
	RHEngine::SceneManager::Instance().SetStartScene("Title");
}
