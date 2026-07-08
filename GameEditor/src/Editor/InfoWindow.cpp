//====================================================//
// ファイル名  : InfoWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/07
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "InfoWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "System/WindowManager.h"

#include "Input/MouseInput.h"

#include "Saver/ObjectSaver.h"
#include "Loader/ObjectLoader.h"
#include "Common/OpenFileDialog.h"

#include "Editor/EditGUI.h"
#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

bool InfoWindow::DrawInfo()
{
	bool clicked = StartInfo();

	Draw();

	ImGui::End();

	return clicked;
}

bool InfoWindow::StartInfo()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), 0));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (29.0f / 54), WindowManager::Instance().GetHeightF() * (1.0f / 12)));

	ImGui::Begin("Info", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	// ウィンドウへのクリックを検知
	return (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0));
}

void InfoWindow::Draw()
{
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

	ImGui::SameLine();
	if (ImGui::Button("Play")) {
		// 再生処理
		m_playFunc();
	}

	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

	ImGui::SameLine();
	if (ImGui::Button("Save")) {

		auto path = FileDialog::Open(FileDialog::Mode::Save, L"Resources/Scenes/", L".scene");

		// セーブ
		if (!path.empty())
		{
			ObjectSaver::SaveSceneToFile(
				path,
				m_pScene
			);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load")) {

		// シーンのリセット
		m_pScene->ResetObjects();

		auto path = FileDialog::Open(FileDialog::Mode::Open, L"Resources/Scenes", L".scene");

		// ロード
		if (!path.empty())
		{
			ObjectLoader::LoadSceneFromFile(
				path,
				m_pScene
			);

			m_pGUI->Reset();
		}
	}

	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

	ImGui::SameLine(); // 次のアイテムを同じ行に配置
	ImGui::Text("Cursor %d %d", (int)Input::Mouse::GetMousePoint().x, (int)Input::Mouse::GetMousePoint().y);
}
