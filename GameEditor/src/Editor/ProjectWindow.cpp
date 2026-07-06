//====================================================//
// ファイル名  : ProjectWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ProjectWindow.h"

#include "imgui/imgui.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ProjectWindow::DrawProject()
{
	// 描画開始
	StartProject();


	// 終了
	ImGui::End();
}

void ProjectWindow::StartProject()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (229.0f / 378), 0));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (109.0f / 756), WindowManager::Instance().GetHeightF()));

	ImGui::Begin("Project", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			
	// ウィンドウへのクリックを検知
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0))
	{
		OutputDebugStringA("Clicked Window: Project\n");
	}
}
