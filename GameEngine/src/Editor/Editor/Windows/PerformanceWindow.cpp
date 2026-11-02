/**
 * @file    PerformanceWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "PerformanceWindow.h"

#include "System/PerformanceStats.h"
#include "ThirdParty/imgui/imgui.h"

namespace RHEngine
{
	void PerformanceWindow::DrawWindow()
	{
		ImGui::Begin("Performance");

		ImGui::BeginDisabled();

		// FPSの表示
		float fps = PerformanceStats::Instance().GetFPS();
		ImGui::DragFloat("FPS", &fps);

		ImGui::EndDisabled();

		ImGui::End();
	}
}	// namespace RHEngine

#endif	// ENGINE_GUI
