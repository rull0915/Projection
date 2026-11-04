/**
 * @file    ToolBarWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "ToolBarWindow.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_internal.h"

#include "Editor/Loader/ObjectLoader.h"
#include "Editor/Saver/ObjectSaver.h"
#include "Common/OpenFileDialog.h"

#include "Scene/Scene.h"

namespace RHEngine
{
	ToolBarWindow::Result ToolBarWindow::DrawToolbar(ID3D11ShaderResourceView* pEngineLogo, bool isPlaying)
	{
		StartToolBarWindow();

		Result r = DrawToolBarWindow(pEngineLogo, isPlaying);

		ImGui::PopStyleVar(1);

		ImGui::End();

		return r;
	}

	void ToolBarWindow::StartToolBarWindow()
	{
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |	// タイトルバーなし
			ImGuiWindowFlags_NoResize |		// サイズ変更不可
			ImGuiWindowFlags_NoMove |		// 移動不可
			ImGuiWindowFlags_NoDocking |		// ドッキング対象外
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoScrollbar;

		// 0,0にウィンドウを配置
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		// xは画面全体、yは40でウィンドウを作成
		ImGui::SetNextWindowSize(ImVec2(
			ImGui::GetMainViewport()->Size.x,
			TOOL_BAR_HEIGHT
		));

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowPadding, { 4.0f, 4.0f });

		ImGui::Begin("Toolbar", nullptr, flags);
	}

	ToolBarWindow::Result ToolBarWindow::DrawToolBarWindow(ID3D11ShaderResourceView* pEngineLogo, bool isPlaying)
	{
		// ロゴを表示
		ImGui::Image((ImTextureID)pEngineLogo, { 25.0f, 25.0f });

		ImGui::SameLine();

		ImGui::PushFont(ImGui::GetFont(), 20.0f);

		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 textSize = ImGui::CalcTextSize("Play");

		ImGui::SetCursorPosY((size.y - textSize.y) * 0.5f);

		ImGui::Text("RHEngine");

		ImGui::PopFont();

		ImGui::SameLine();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

		ImGui::SameLine();

		// プレイ中でなければ
		if (!isPlaying)
		{
			if (ImGui::Button("Play"))
			{
				// テストプレイ開始
				m_playFunc();
			}
		}

		// プレイ中なら
		else
		{
			if (ImGui::Button("Stop"))
			{
				// テストプレイ終了
				m_stopFunc();
			}
		}

		ImGui::SameLine();

		// セーブボタン
		if (ImGui::Button("Save"))
		{
			// テストプレイ終了
			Save();
		}

		ImGui::SameLine();

		// ロードボタン
		if (ImGui::Button("Load"))
		{
			// テストプレイ終了
			Load();

			return Result::PressLoadButton;
		}

		return Result::None;
	}

	void ToolBarWindow::Save()
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

	void ToolBarWindow::Load()
	{
		// シーンのリセット
		m_pScene->Finalize();

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

		// 初期化
		m_pScene->Initialize();
	}
}	// namespace RHEngine

#endif	// ENGINE_GUI
