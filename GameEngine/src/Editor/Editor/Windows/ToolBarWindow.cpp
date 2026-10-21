//====================================================//
// ファイル名  : ToolBarWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/21
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ToolBarWindow.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_internal.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void RHEngine::ToolBarWindow::DrawToolbar(ID3D11ShaderResourceView* engineLogo, bool isPlaying)
{
	StartToolBarWindow();

	DrawToolBarWindow(engineLogo, isPlaying);

	ImGui::PopStyleVar(1);

	ImGui::End();
}

void RHEngine::ToolBarWindow::StartToolBarWindow()
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

void RHEngine::ToolBarWindow::DrawToolBarWindow(ID3D11ShaderResourceView* engineLogo, bool isPlaying)
{
	// ロゴを表示
	ImGui::Image((ImTextureID)engineLogo, { 25.0f, 25.0f });

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
}
