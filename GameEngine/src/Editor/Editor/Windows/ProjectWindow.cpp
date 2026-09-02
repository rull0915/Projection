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
#include "ProjectWindow.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_stdlib.h"
#include "System/WindowManager.h"
#include "../HandlePayload.h"

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
		DrawCreatePopup();
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
			DrawFileOperation(path, true);

			ImGui::EndPopup();
		}

		if (m_openCreatePopup)
		{
			// 新規作成ポップアップを出す
			ImGui::OpenPopup("Create New Asset");

			// 文字列を渡す
			m_createDirectory = path;

			// 文字列の初期化
			m_popupStr = "";

			// フラグ解除
			m_openCreatePopup = false;
		}

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
					// ファイル名を取得
					std::string fileName = file.path().filename().string();

					// 初期状態のフラグ
					ImGuiTreeNodeFlags flags = ImGuiSelectableFlags_AllowDoubleClick;

					// 選択されているか
					bool select = m_selectedPath == file.path();

					// 出力
					if (ImGui::Selectable(fileName.c_str(), select, flags))
					{
						// パスからハンドルを取得
						UnTypeHandle handle = m_assetManager.LoadFromUUID(m_assetManager.GetDataBase().GetUUID(file.path().wstring()));

						// 選択
						m_selectedPath = file.path();

						// 左ボタンがダブルクリックされていれば
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							// シーン以外なら
							if (file.path().extension() != L".scene")
								// 選択状態にする
								m_selected.SetSelectedHandle(handle);

							// シーンなら
							else
								// Todo: ロードする
								int a;
						}
					}

					// ドラッグ可能に
					if (ImGui::BeginDragDropSource())
					{
						// パスからUUIDを取得
						UnTypeHandle handle = m_assetManager.LoadFromUUID(m_assetManager.GetDataBase().GetUUID(file.path().wstring()));

						// 受け渡し構造体を生成
						HandlePayload payload = { m_assetManager.GetTypeManager().GetAssetClass(file.path().wstring()), handle };

						// データを設定
						ImGui::SetDragDropPayload("ASSET", &payload, sizeof(payload));

						// ドラッグ中に表示される内容
						ImGui::Text(file.path().stem().string().c_str());

						// ドラッグの終了
						ImGui::EndDragDropSource();
					}

					// 右クリック時にメニューを表示
					if (ImGui::BeginPopupContextItem())
					{
						// ファイル操作メニューを表示
						DrawFileOperation(file, false);

						ImGui::EndPopup();
					}

					if (m_openRenamePopup)
					{
						// リネームポップアップを出す
						ImGui::OpenPopup("Rename Asset");

						// 文字列を渡す
						m_targetPath = file.path();
						m_popupStr = m_targetPath.stem().string();

						// フラグ解除
						m_openRenamePopup = false;
					}
				}
			}

			// 名前変更ポップアップの表示
			DrawRenamePopup();

			// 新規作成ポップアップの表示
			DrawCreatePopup();

			// ツリーの終了
			ImGui::TreePop();
		}
	}

	void ProjectWindow::DrawFileOperation(const std::filesystem::path& path, bool isDirectory)
	{
		// ディレクトリの場合
		if (isDirectory)
		{
			// 新規作成を表示
			if (ImGui::BeginMenu("Create"))
			{
				// メニューとして表示
				if (ImGui::MenuItem("Folder"))
				{
					// 新規作成ポップアップを表示
					m_createType = "Folder";

					m_openCreatePopup = true;
				}

				// 作成可能アセットを取得
				for (auto& assetType : m_assetManager.GetCreatableAssets())
				{
					// メニューとして表示
					if (ImGui::MenuItem(assetType.c_str()))
					{
						// 新規作成ポップアップを表示
						m_createType = assetType;
						
						m_openCreatePopup = true;
					}
				}

				ImGui::EndMenu();
			}

			//// Renameを表示
			//if (ImGui::MenuItem("Rename"))
			//{
			//	// フラグを立てる
			//	m_openRenamePopup = true;
			//}

			//// Delete表示
			//if (ImGui::MenuItem("Delete"))
			//{
			//}
		}
		else
		{
			// Renameを表示
			if (ImGui::MenuItem("Rename"))
			{
				// フラグを立てる
				m_openRenamePopup = true;
			}

			// Delete表示
			if (ImGui::MenuItem("Delete"))
			{
				// 削除関数を呼ぶ
				m_assetManager.GetDataBase().Delete(path);
			}
		}
	}

	void ProjectWindow::DrawRenamePopup()
	{
		if (ImGui::BeginPopupModal("Rename Asset"))
		{
			ImGui::InputText("Name", &m_popupStr);

			// 承諾されたら
			if (ImGui::Button("Accept"))
			{
				// 変更後のパスを作成
				std::filesystem::path nextPath = m_targetPath.parent_path();	// フォルダまでのパスを生成
				nextPath /= (m_popupStr + m_targetPath.extension().string());	// ファイル名を追加

				// 選択中なら選択パスも変更
				if (m_targetPath == m_selectedPath) m_selectedPath = nextPath;

				// リネーム
				m_assetManager.GetDataBase().ReName(m_targetPath, nextPath);

				// 閉じる
				ImGui::CloseCurrentPopup();
			}

			// 同じ行に描画
			ImGui::SameLine();

			// キャンセルされたら
			if (ImGui::Button("Cancel"))
			{
				// 何もせず閉じる
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void ProjectWindow::DrawCreatePopup()
	{
		if (ImGui::BeginPopupModal("Create New Asset"))
		{
			ImGui::InputText("Name", &m_popupStr);

			// 承諾されたら
			if (ImGui::Button("Accept"))
			{
				if (m_createType == "Folder")
				{
					// 作成
					std::filesystem::create_directory(m_createDirectory /= m_popupStr);
				}
				else
				{
					// 作成
					m_assetManager.Create(m_createDirectory, m_popupStr, m_createType);
				}

				// 閉じる
				ImGui::CloseCurrentPopup();
			}

			// 同じ行に描画
			ImGui::SameLine();

			// キャンセルされたら
			if (ImGui::Button("Cancel"))
			{
				// 何もせず閉じる
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}	// namespace REngine
