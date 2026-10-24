//====================================================//
// ファイル名  : PerformanceWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/23
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#ifdef ENGINE_GUI

#include "PerformanceWindow.h"

#include "System/PerformanceStats.h"
#include "ThirdParty/imgui/imgui.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void RHEngine::PerformanceWindow::DrawWindow()
{
	ImGui::Begin("Performance");
	
	ImGui::BeginDisabled();

	// FPSの表示
	float fps = PerformanceStats::Instance().GetFPS();
	ImGui::DragFloat("FPS", &fps);

	ImGui::EndDisabled();

	ImGui::End();
}

#endif
