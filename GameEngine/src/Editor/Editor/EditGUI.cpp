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
#include "Input/KeyInput.h"

#include "System/WindowManager.h"

#include "Windows/HierarchyWindow.h"
#include "Windows/Project/ProjectWindow.h"
#include "Windows/InfoWindow.h"
#include "Windows/Inspector/InspectorWindow.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	EditGUI::EditGUI(Scene* pScene, AssetManager& assetManager, std::function<void()> playFunc)
		: m_pScene{ pScene }
		, m_assetmanager{ assetManager }
		, m_hierarchy{ std::make_unique<HierarchyWindow>(pScene, m_selected) }
		, m_inspector{ std::make_unique<InspectorWindow>(pScene, assetManager, m_selected) }
		, m_project{ std::make_unique<ProjectWindow>(assetManager, m_selected) }
		, m_info{ std::make_unique<InfoWindow>(assetManager, pScene, this, playFunc) }
		, m_nowType{ WindowType::None }
		, m_sceneDrawSetting{ static_cast<unsigned char>(-1) }	// 全フラグを立てる
		, m_guizmoManager{}
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
		// Infoの描画
		if (m_info->DrawInfo()) m_nowType = WindowType::Info;

		// Hierarchyの描画
		if (m_hierarchy->DrawHierarchy()) m_nowType = WindowType::Hieraychy;

		// Projectの描画
		if (m_project->DrawProject()) m_nowType = WindowType::Project;

		// Inspectorの描画
		if (m_inspector->DrawInspector()) m_nowType = WindowType::Inspector;
	}

	void EditGUI::DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView, SceneCamera* sceneViewCamera)
	{
		// キー入力を取得し描画するギズモを変える
		if (Input::Key::GetDown(Input::Key::Code::W)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_TRANSLATION);
		if (Input::Key::GetDown(Input::Key::Code::E)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_ROTATION);
		if (Input::Key::GetDown(Input::Key::Code::R)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_SCALE);

		// Sceneビューの開始
		StartSceneView();

		// 描画
		DrawSceneView(sceneView, sceneViewCamera);

		// 終了
		ImGui::End();

		// Gameビューの開始
		StartGameView();

		// 描画
		DrawImage(gameView);

		// 終了
		ImGui::End();
	}

	void EditGUI::DrawImageDebugWindow(std::string_view title, ID3D11ShaderResourceView* image) const
	{
		ImGui::Begin(title.data());

		ImGui::Image(image, ImGui::GetContentRegionAvail());

		ImGui::End();
	}

	void EditGUI::DrawImage(ID3D11ShaderResourceView* img)
	{
		// 有効サイズ
		ImVec2 avail = ImGui::GetContentRegionAvail();

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

		ImGui::Image(
			(ImTextureID)img,
			avail);
	}

	void EditGUI::StartSceneView()
	{
		// 位置を固定
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 2));

		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);

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

	void EditGUI::DrawSceneView(ID3D11ShaderResourceView* sceneView, SceneCamera* sceneViewCamera)
	{
		// 描画
		DrawImage(sceneView);

		if (auto* g = dynamic_cast<GameObject*>(m_selected.GetSelected()))
		{
			// ギズモ描画
			if (auto* t = g->GetComponent<Transform>())
			{
				// スクリーンサイズを取得
				ImVec2 pos = ImGui::GetWindowPos();
				ImVec2 size = ImGui::GetWindowSize();

				m_guizmoManager.DrawTransformGuizmo(sceneViewCamera, t, { pos.x, pos.y }, { size.x, size.y });
			}
		}
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
