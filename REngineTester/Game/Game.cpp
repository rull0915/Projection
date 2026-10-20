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
#include "Components/RandomGenerator.h"
#include "Components/JumpComponent.h"

#include "GameObject/GameObject.h"
#include "Editor/Loader/ComponentFactory.h"

#define REGIST_TO_FACTORY(Name) (RHEngine::ComponentFactory::Register(#Name, { RHEngine::ComponentProject::Game, Name::SPACE }, [](RHEngine::GameObject* o){ return o->AddComponent<Name>(); }))

void Game::OnInitialize()
{
	// コンポーネントの登録
	REGIST_TO_FACTORY(RandomGenerator);
	REGIST_TO_FACTORY(JumpComponent);

	// ====== シーンの登録 ====== //
	RHEngine::SceneManager::Instance().RegisterScene("Title", L"Resources/Scenes/TitleScene.scene");

	RHEngine::SceneManager::Instance().RegisterScene("GUITest", L"Resources/Scenes/GUITest.scene");

	// 開始時のシーンを設定
	RHEngine::SceneManager::Instance().SetStartScene("GUITest");
}
