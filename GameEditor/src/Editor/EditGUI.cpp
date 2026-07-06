//====================================================//
// ファイル名  : EditGUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Editor/EditGUI.h"

#include "imgui/imgui.h"

#include "System/WindowManager.h"

#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "ProjectWindow.h"

//====================================================//
// 関数の実体宣言
//====================================================//

EditGUI::EditGUI(Scene* pScene)
	: m_pScene{ pScene }
	, m_hierarchy{ std::make_unique<HierarchyWindow>(pScene) }
	, m_inspector{ std::make_unique<InspectorWindow>() }
	, m_project{ std::make_unique<ProjectWindow>() }
{}

EditGUI::~EditGUI()
{}

void EditGUI::DrawWindows()
{
	// Hierarchyの描画
	m_hierarchy->DrawHierarchy();

	// Projectの描画
	m_project->DrawProject();

	// Inspectorの描画
	m_inspector->DrawInspector(m_hierarchy->GetSelected());
}

void EditGUI::DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView)
{
	// Sceneビューの開始
	StartSceneView();

	// 描画
	DrawImage(sceneView, { 576, 324 });

	// 終了
	ImGui::End();

	// Gameビューの開始
	StartGameView();

	// 描画
	DrawImage(gameView, { 576, 324 });

	// 終了
	ImGui::End();
}

void EditGUI::DrawImage(ID3D11ShaderResourceView* img, DirectX::SimpleMath::Vector2 position)
{
	ImGui::Image(
		(ImTextureID)img,
		{ position.x, position.y });
}

void EditGUI::StartSceneView()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 2));

	ImGui::Begin("Scene", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		
	// ウィンドウへのクリックを検知
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0))
	{
		OutputDebugStringA("Clicked Window: SceneView\n");
	}
}

void EditGUI::StartGameView()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(0, WindowManager::Instance().GetHeightF() / 2));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 2));

	ImGui::Begin("GameView", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	
	// ウィンドウへのクリックを検知
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0))
	{
		OutputDebugStringA("Clicked Window: GameView\n");
	}
}
