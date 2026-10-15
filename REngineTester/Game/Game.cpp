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

void Game::OnInitialize()
{
	// ====== シーンの登録 ====== //
	REngine::SceneManager::Instance().RegisterScene("Title", L"Resources/Scenes/TitleScene.scene");

	REngine::SceneManager::Instance().RegisterScene("GUITest", L"Resources/Scenes/GUITest.scene");

	// 開始時のシーンを設定
	REngine::SceneManager::Instance().SetStartScene("Title");
}
