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
#include "System/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

bool ProjectWindow::DrawProject()
{
	// 描画開始
	bool clicked = StartProject();

	// 描画
	DrawResources();

	// 終了
	ImGui::End();

	return clicked;
}

bool ProjectWindow::StartProject()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (229.0f / 378), WindowManager::Instance().GetHeightF() / 12));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (109.0f / 756), WindowManager::Instance().GetHeightF() * (11.0f / 12)));

	ImGui::Begin("Project", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			
	// ウィンドウへのクリックを検知
	return (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0));
}

void ProjectWindow::DrawResources()
{
	ImGui::Text("Resources");

	ImGui::BeginChild("Resources", ImVec2(0, -WindowManager::Instance().GetHeightF() / 3), ImGuiChildFlags_Borders);

	if (ImGui::TreeNodeEx("Models", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// モデル
		for (auto& model : ResourceManager::Instance().GetAllModels())
		{
			ImGui::Text(model.first.c_str());
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Textures", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// テクスチャ
		for (auto& model : ResourceManager::Instance().GetAllTextures())
		{
			ImGui::Text(model.first.c_str());
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Sounds", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// サウンド
		for (auto& model : ResourceManager::Instance().GetAllSounds())
		{
			ImGui::Text(model.first.c_str());
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Fonts", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// フォント
		for (auto& model : ResourceManager::Instance().GetAllFonts())
		{
			ImGui::Text(model.first.c_str());
		}

		ImGui::TreePop();
	}

	ImGui::EndChild();
}
