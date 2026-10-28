/**
 * @file    GameController.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/09/01
 */

#include "pch.h"
#include "GameController.h"

#include <Windows.h>

void RHEngine::GameController::Quit()
{
	PostQuitMessage(0);
}
