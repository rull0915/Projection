//====================================================//
// ファイル名  : ProjectWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"

#ifdef ENGINE_GUI

#include "ProjectWindow.h"

#include "ThirdParty/imgui/imgui.h"
#include "System/WindowManager.h"
#include "../../HandlePayload.h"

#include "GameObject/GameObject.h"
#include "Editor/Saver/ObjectSaver.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool ProjectWindow::DrawProject()
	{
		// 描画開始
		bool clicked = StartProject();

		// 描画
		DrawResources();

		// 終了
		ImGui::End();

		return clicked;
	}

	bool ProjectWindow::StartProject()
	{
		// 位置とサイズを固定
		ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (229.0f / 378), WindowManager::Instance().GetHeightF() / 12));
		ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (109.0f / 756), WindowManager::Instance().GetHeightF() * (11.0f / 12)));

		ImGui::Begin("Project", nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		// ウィンドウへのクリックを検知
		return (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
			&& ImGui::IsMouseClicked(0));
	}

	void ProjectWindow::DrawResources()
	{
		// ファイル構造を再帰的に表示する

		// ルートフォルダを設定
		std::wstring root = L"Resources";

		DrawFileStructure(root);
		
		// 新規作成ポップアップの表示
		m_popUp.DrawCreatePopup();
	}

	void ProjectWindow::DrawFileStructure(const std::filesystem::path& path)
	{
		// フォルダ名でツリーを開始
		bool open = (ImGui::TreeNode(path.stem().string().c_str()));

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				// GameObjectに変換
				auto data = (GameObject*)payload->Data;

				// Object名を取得
				std::string name = data->GetName();

				// ファイル名を生成
				std::filesystem::path file = std::filesystem::path(path.string() + "\\" + (name.empty() ? "GameObject" : name) + ".gameobject");

				// セーバーの生成
				ObjectSaver saver(m_assetManager);

				// 保存
				saver.SaveObjectToFile(file, data);
			}

			ImGui::EndDragDropTarget();
		}

		// 右クリック時にメニューを表示
		if (ImGui::BeginPopupContextItem())
		{
			// ファイル操作メニューを表示
			m_popUp.DrawFileOperation(path, true);

			ImGui::EndPopup();
		}

		m_popUp.OpenCreatePopUp(path);

		if (open)
		{
			// ディレクトリ直下に含まれるファイルを走査
			for (const auto& file : std::filesystem::directory_iterator(path))
			{
				// .auxファイルなら何もしない
				if (file.path().extension() == ".aux") continue;

				// フォルダなら
				if (file.is_directory())
				{
					// 再帰的に表示
					DrawFileStructure(file.path());
				}
				// 通常のファイルなら
				else
				{
					// ファイル表示
					auto result = m_assetFileDrawer.DrawAssetFile(file.path(), file.path() == m_selectedPath);

					// 操作が起きていれば選択パスを上書き
					if (result != AssetFileDrawer::Result::None)
					{
						m_selectedPath = file.path();
					}

					// ダブルクリックされていれば選択
					if (result == AssetFileDrawer::Result::DoubleClick)
					{
						m_selected.SetSelectedHandle(m_assetManager.LoadFromUUID(m_assetManager.GetDataBase().GetUUID(file.path())));
					}

					// 右クリック時にメニューを表示
					if (ImGui::BeginPopupContextItem())
					{
						// ファイル操作メニューを表示
						m_popUp.DrawFileOperation(file, false);

						ImGui::EndPopup();
					}

					m_popUp.OpenRenamePopUp(file.path());
				}
			}

			// 名前変更ポップアップの表示
			m_popUp.DrawRenamePopup();

			// 新規作成ポップアップの表示
			m_popUp.DrawCreatePopup();

			// ツリーの終了
			ImGui::TreePop();
		}
	}
}	// namespace REngine

#endif // ENGINE_GUI
