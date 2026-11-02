/**
 * @file    Game.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/11
 *
 * @brief   ゲームクラス
 */

#pragma once

#include "EntryPoint.h"
#include "Application.h"

/**
 * @brief ゲーム本体
 * @details RHEngineに用意されているApplicationを継承する
 */
class Game : public RHEngine::Application {
public:
    Game()
        : Application({ L"Projection", 1280, 720 }) {}

	void OnInitialize() override;
	void OnFinalize() override {};
};

RHEngine::Application* RHEngine::CreateApplication() {
	return new Game();
}
