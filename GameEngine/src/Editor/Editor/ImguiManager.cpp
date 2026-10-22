//====================================================//
// ファイル名  : ImguiManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/04
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"

#ifdef ENGINE_GUI

#include "Editor/Editor/ImguiManager.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/ImGuizmo.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// imguiの初期化
	void ImguiManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
	{
		//  バージョンの確認
		IMGUI_CHECKVERSION();

		//  コンテキストの作成
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		// 日本語
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // ドッキング可能に

		//  Win32用の初期化
		ImGui_ImplWin32_Init(hwnd);
		//  DirectX11用の初期化
		ImGui_ImplDX11_Init(device, context);

		// スタイルの変更
		ChangeGUIStyle();
	}

	// フレーム開始時処理
	void ImguiManager::Update()
	{
		// 新フレームの開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	// フレーム終了時処理
	void ImguiManager::Render()
	{
		//  ImGuiの描画処理
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	// 終了処理
	void ImguiManager::Finalize()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	// メッセージ
	bool ImguiManager::ProcessMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		return (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wp, lp));
	}

	void ImguiManager::ChangeGUIStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// ウィンドウ・フレームの角丸
		style.WindowRounding = 0.0f;
		style.ChildRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 6.0f;
		style.GrabRounding = 6.0f;

		// ツリーの線を表示
		style.TreeLinesFlags = ImGuiTreeNodeFlags_DrawLinesFull;

		// 余白
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 6.0f);

		// 標準のダークテーマを適用
		ImGui::StyleColorsDark();

		// 色をカスタマイズ
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
		colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		colors[ImGuiCol_CheckboxSelectedBg] = ImVec4(0.42f, 0.42f, 0.57f, 0.58f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
		colors[ImGuiCol_Button] = ImVec4(0.64f, 0.69f, 0.97f, 0.57f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.48f, 0.71f, 0.79f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.54f, 0.80f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.63f, 0.63f, 0.89f, 0.45f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.41f, 0.41f, 0.64f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
		colors[ImGuiCol_InputTextCursor] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		colors[ImGuiCol_Tab] = ImVec4(0.34f, 0.34f, 0.68f, 0.79f);
		colors[ImGuiCol_TabSelected] = ImVec4(0.40f, 0.40f, 0.73f, 0.84f);
		colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		colors[ImGuiCol_TabDimmed] = ImVec4(0.28f, 0.28f, 0.57f, 0.82f);
		colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.35f, 0.35f, 0.65f, 0.84f);
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.53f, 0.53f, 0.87f, 0.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.27f, 0.27f, 0.38f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.45f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
		colors[ImGuiCol_TextLink] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		colors[ImGuiCol_TreeLines] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.70f, 0.73f, 0.90f);
		colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.00f, 0.31f, 1.00f, 0.09f);
		colors[ImGuiCol_UnsavedMarker] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_NavCursor] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
