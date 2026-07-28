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
#include "Editor/Editor/EditGUI.h"
#include "ThirdParty/imgui/imgui_internal.h"

#include "ThirdParty/imgui/imgui.h"
#include "Input/KeyInput.h"

#include "System/WindowManager.h"
#include "System/Render/RenderContext.h"

#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "ProjectWindow.h"
#include "InfoWindow.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	EditGUI::EditGUI(Scene* pScene, AssetManager& assetManager, std::function<void()> playFunc)
		: m_pScene{ pScene }
		, m_assetmanager{ assetManager }
		, m_hierarchy{ std::make_unique<HierarchyWindow>(pScene) }
		, m_inspector{ std::make_unique<InspectorWindow>(assetManager) }
		, m_project{ std::make_unique<ProjectWindow>(assetManager) }
		, m_info{ std::make_unique<InfoWindow>(pScene, this, playFunc) }
		, m_nowType{ WindowType::None }
		, m_sceneDrawSetting{ static_cast<unsigned char>(-1) }	// 全フラグを立てる
		, m_guizmoManager{}
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

	void EditGUI::DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView, CameraBase* sceneViewCamera)
	{
		// キー入力を取得し描画するギズモを変える
		if (Input::Key::Get(Input::State::Down, Input::Key::Code::W)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_TRANSLATION);
		if (Input::Key::Get(Input::State::Down, Input::Key::Code::E)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_ROTATION);
		if (Input::Key::Get(Input::State::Down, Input::Key::Code::R)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_SCALE);

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

	void EditGUI::DrawSceneView(ID3D11ShaderResourceView* sceneView, CameraBase* sceneViewCamera)
	{
		// 描画
		ImGui::Text("SceneView");

		// 同ライン
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 3.0f);

		ImGui::SameLine();

		// 右側に並べる要素の合計幅を計算
		const ImGuiStyle& style = ImGui::GetStyle();

		float totalWidth = 0.0f;

		totalWidth += ImGui::CalcTextSize("World").x + ImGui::GetFrameHeight();
		totalWidth += style.ItemInnerSpacing.x;

		totalWidth += ImGui::CalcTextSize("UI").x + ImGui::GetFrameHeight();
		totalWidth += style.ItemInnerSpacing.x;

		totalWidth += ImGui::CalcTextSize("Debug").x + ImGui::GetFrameHeight();
		totalWidth += style.ItemInnerSpacing.x;

		// 右寄せ
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - totalWidth - 30);

		bool drawWorld = m_sceneDrawSetting & DrawFlag::World;
		bool drawUI = m_sceneDrawSetting & DrawFlag::UI;
		bool drawDebug = m_sceneDrawSetting & DrawFlag::WorldDebug;

		// 描画
		ImGui::Checkbox("World", &drawWorld);

		ImGui::SameLine();
		ImGui::Checkbox("UI", &drawUI);

		ImGui::SameLine();
		ImGui::Checkbox("Debug", &drawDebug);

		m_sceneDrawSetting = (m_sceneDrawSetting & ~DrawFlag::World) | (drawWorld ? DrawFlag::World : 0);
		m_sceneDrawSetting = (m_sceneDrawSetting & ~DrawFlag::UI) | (drawUI ? DrawFlag::UI : 0);
		m_sceneDrawSetting = (m_sceneDrawSetting & ~DrawFlag::WorldDebug) | (drawDebug ? DrawFlag::WorldDebug : 0);

		// 描画
		DrawImage(sceneView);

		if (auto* g = dynamic_cast<GameObject*>(m_hierarchy->GetSelected()))
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
}	// namespace REngine
