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
#include "Editor/Editor/EditGUI.h"
#include "imgui/imgui_internal.h"

#include "imgui/imgui.h"
#include "Input/KeyInput.h"

#include "System/WindowManager.h"

#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "ProjectWindow.h"
#include "InfoWindow.h"

//====================================================//
// 関数の実体宣言
//====================================================//

EditGUI::EditGUI(Scene* pScene, std::function<void()> playFunc)
	: m_pScene{ pScene }
	, m_hierarchy{ std::make_unique<HierarchyWindow>(pScene) }
	, m_inspector{ std::make_unique<InspectorWindow>() }
	, m_project{ std::make_unique<ProjectWindow>() }
	, m_info{ std::make_unique<InfoWindow>(pScene, this, playFunc) }
	, m_nowType{ WindowType::None }
{}

EditGUI::~EditGUI()
{}

void EditGUI::Reset()
{
	// Hierarchyのリセット
	m_hierarchy->Reset();
}

void EditGUI::DrawWindows()
{
	// Infoの描画
	if (m_info->DrawInfo()) m_nowType = WindowType::Info;

	// Hierarchyの描画
	if (m_hierarchy->DrawHierarchy()) m_nowType = WindowType::Hieraychy;

	// Projectの描画
	if (m_project->DrawProject()) m_nowType = WindowType::Project;

	// Inspectorの描画
	if (m_inspector->DrawInspector(m_hierarchy->GetSelected())) m_nowType = WindowType::Inspector;
}

void EditGUI::DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView)
{
	// Sceneビューの開始
	StartSceneView();

	// 描画
	ImGui::Text("こんにちは");

	// 同ライン
	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

	ImGui::SameLine();

	// 右側に並べる要素の合計幅を計算
	const ImGuiStyle& style = ImGui::GetStyle();

	float totalWidth = 0.0f;

	totalWidth += ImGui::CalcTextSize("Grid").x + ImGui::GetFrameHeight();
	totalWidth += style.ItemInnerSpacing.x;

	totalWidth += ImGui::CalcTextSize("Gizmo").x + ImGui::GetFrameHeight();
	totalWidth += style.ItemInnerSpacing.x;

	totalWidth += 60.0f; // Resetボタンの幅

	// 右寄せ
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - totalWidth);

	static bool grid;
	static bool gizmo;

	// 描画
	ImGui::Checkbox("Grid", &grid);

	ImGui::SameLine();
	ImGui::Checkbox("Gizmo", &gizmo);

	ImGui::SameLine();
	ImGui::Button("Reset", ImVec2(45, 0));

	// 描画
	DrawImage(sceneView);

	// 終了
	ImGui::End();

	// Gameビューの開始
	StartGameView();

	// 描画
	DrawImage(gameView);

	// 終了
	ImGui::End();
}

void EditGUI::DrawImage(ID3D11ShaderResourceView* img)
{
	// 有効サイズ
	ImVec2 avail = ImGui::GetContentRegionAvail();

	if (Input::Key::Get(Input::State::Press, Input::Key::Code::LeftShift))
	{
		// アスペクト比
		float aspect = WindowManager::Instance().GetAspect();

		// 小さい方に合わせる
		float correction = avail.x / aspect;

		if (correction > avail.y)
		{
			avail.x = avail.y * aspect;
		}
		else if (correction < avail.y)
		{
			avail.y = avail.x / aspect;
		}
	}

	ImGui::Image(
		(ImTextureID)img,
		avail);
}

void EditGUI::StartSceneView()
{
	// 位置を固定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
//	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 2));

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		
	// ウィンドウへのクリックを検知
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0))
	{
		m_nowType = WindowType::SceneView;
	}
}

void EditGUI::StartGameView()
{
	// 位置とサイズを固定
	ImGui::SetNextWindowPos(ImVec2(0, WindowManager::Instance().GetHeightF() / 2));
	ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 2));

	ImGui::Begin("GameView", nullptr, ImGuiWindowFlags_NoMove);
	
	// ウィンドウへのクリックを検知
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
		&& ImGui::IsMouseClicked(0))
	{
		m_nowType = WindowType::GameView;
	}
}
