//====================================================//
// ファイル名   : Game.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要 : ゲームクラス
//
// 更新履歴 :
// 2026/06/02 新規作成
// 2026/10/14 Engine側のApplicationを継承する設計に変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "EntryPoint.h"
#include "Application.h"

// ゲーム本体クラス
class Game : public RHEngine::Application {
public:
    Game()
        : Application({ L"My Game Project", 1280, 720 }) {}

	void OnInitialize() override;
	void OnFinalize() override {};
};

RHEngine::Application* RHEngine::CreateApplication() {
	return new Game();
}
