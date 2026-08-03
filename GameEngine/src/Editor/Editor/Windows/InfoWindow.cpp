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

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_internal.h"
#include "System/WindowManager.h"
#include "System/GraphicsManager.h"

#include "Input/MouseInput.h"

#include "Editor/Saver/ObjectSaver.h"
#include "Editor/Loader/ObjectLoader.h"
#include "Common/OpenFileDialog.h"

#include "Editor/Editor/EditGUI.h"
#include "Scene/Scene.h"

namespace REngine
{
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
			Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {

			HWND hwnd = GraphicsManager::Instance().GetDeviceResources()->GetWindow();

			int result = MessageBox(
				hwnd,
				L"現在のシーンを保存しますか？",
				L"確認",
				MB_YESNO | MB_ICONQUESTION
			);

			// 肯定が押されたら
			if (result == IDYES)
			{
				// 保存
				Save();
			}

			Load();
		}

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

		ImGui::SameLine(); // 次のアイテムを同じ行に配置
		ImGui::Text("Cursor %d %d", (int)Input::Mouse::GetMousePoint().x, (int)Input::Mouse::GetMousePoint().y);
	}

	void InfoWindow::Save()
	{
		auto path = FileDialog::Open(FileDialog::Mode::Save, L"Resources/Scenes/", L".scene");

		// セーブ
		if (!path.empty())
		{
			ObjectSaver saver(m_assetManager);

			saver.SaveSceneToFile(
				path,
				m_pScene
			);
		}
	}

	void InfoWindow::Load()
	{
		// シーンのリセット
		m_pScene->ResetObjects();

		// GUIのリセット
		m_pGUI->Reset();

		auto path = FileDialog::Open(FileDialog::Mode::Open, L"Resources/Scenes", L".scene");

		// ロード
		if (!path.empty())
		{
			ObjectLoader loader(m_assetManager);

			loader.LoadSceneFromFile(
				path,
				m_pScene
			);
		}
	}
}	// namespace REngine
