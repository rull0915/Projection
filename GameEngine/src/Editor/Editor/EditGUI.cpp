//====================================================//
// ファイル名  : EditGUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : エディタ画面のGUIを管理するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"

#ifdef ENGINE_GUI

#include "Editor/Editor/EditGUI.h"
#include "ThirdParty/imgui/imgui.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/Project/ProjectWindow.h"
#include "Windows/Inspector/InspectorWindow.h"
#include "Windows/ToolBarWindow.h"
#include "Windows/ImageWindow.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	EditGUI::EditGUI(Scene* pScene, AssetManager& assetManager, std::function<void()> playFunc, std::function<void()> stopFunc)
		: m_pScene{ pScene }
		, m_assetmanager{ assetManager }
		, m_hierarchy{ std::make_unique<HierarchyWindow>(pScene, m_selected) }
		, m_inspector{ std::make_unique<InspectorWindow>(pScene, assetManager, m_selected) }
		, m_project{ std::make_unique<ProjectWindow>(assetManager, m_selected) }
		, m_toolbar{ std::make_unique<ToolBarWindow>(pScene, assetManager, playFunc, stopFunc) }
		, m_images{ std::make_unique<ImageWindow>() }
		, m_sceneDrawSetting{ static_cast<unsigned char>(-1) }	// 全フラグを立てる
		, m_selected{ m_assetmanager }
	{}

	EditGUI::~EditGUI()
	{}

	void EditGUI::Reset()
	{
		// 選択状態のリセット
		m_selected.SetSelected(nullptr);
	}

	void EditGUI::DrawWindows()
	{
		// Hierarchyの描画
		m_hierarchy->DrawHierarchy();

		// Projectの描画
		m_project->DrawProject();

		// Inspectorの描画
		m_inspector->DrawInspector();
	}

	bool EditGUI::DrawSceneView(ID3D11ShaderResourceView* sceneView, SceneCamera* sceneViewCamera)
	{
		if (!sceneView) return false;

		GameObject* obj = dynamic_cast<GameObject*>(m_selected.GetSelected());

		// シーンビューの描画
		return m_images->DrawSceneViewWindow(sceneView, sceneViewCamera, obj ? obj->GetComponent<Transform>() : nullptr);
	}

	bool EditGUI::DrawGameView(ID3D11ShaderResourceView* gameView)
	{
		// ゲームビューの描画
		return m_images->DrawGameViewWindow(gameView);
	}

	void EditGUI::DrawStyleEditor()
	{
		// テスト用
		ImGui::Begin("StyleEditor");
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	void EditGUI::DrawToolBar(ID3D11ShaderResourceView* engineLogo, bool isPlaying)
	{
		switch (m_toolbar->DrawToolbar(engineLogo, isPlaying))
		{
		case ToolBarWindow::Result::PressLoadButton:
			Reset();
			break;
		default:
			break;
		} 
	}

	void EditGUI::DrawDockingArea()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::SetNextWindowPos(ImVec2(
			viewport->Pos.x,
			viewport->Pos.y + ToolBarWindow::TOOL_BAR_HEIGHT
		));

		ImGui::SetNextWindowSize(ImVec2(
			viewport->Size.x,
			viewport->Size.y - ToolBarWindow::TOOL_BAR_HEIGHT
		));

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDocking |		// ドッキング対象外
			ImGuiWindowFlags_NoTitleBar |		// タイトルバーなし
			ImGuiWindowFlags_NoCollapse |		// 最小化不可
			ImGuiWindowFlags_NoResize |			// サイズ変更不可
			ImGuiWindowFlags_NoMove |			// 移動不可
			ImGuiWindowFlags_NoBringToFrontOnFocus |	// クリック時も前面に来ないように
			ImGuiWindowFlags_NoNavFocus |		// ナビゲーション対象外
			ImGuiWindowFlags_NoBackground;		// 背景なし

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		ImGui::PushStyleVar(
			ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpaceWindow", nullptr, flags);

		ImGui::PopStyleVar(3);

		ImGuiID dockspaceId = ImGui::GetID("MainDockSpace");

		ImGui::DockSpace(
			dockspaceId,
			ImVec2(0.0f, 0.0f),
			ImGuiDockNodeFlags_None
		);

		ImGui::End();
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
