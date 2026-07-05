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
#include "Editor/ImguiManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // キーボードによるナビゲーションの有効化
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // コントローラーによるナビゲーションの有効化

	//  Win32用の初期化
	ImGui_ImplWin32_Init(hwnd);
	//  DirectX11用の初期化
	ImGui_ImplDX11_Init(device, context);
}

// フレーム開始時処理
void ImguiManager::Update()
{
	// 新フレームの開始（メインループの一番上に記述）
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
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
