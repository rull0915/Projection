//====================================================//
// ファイル名  : ProjectWindowPopUp.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/08
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ProjectWindowPopUp.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_stdlib.h"
#include "Assets/Managers/AssetManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	void ProjectWindowPopUp::DrawFileOperation(const std::filesystem::path& path, bool isDirectory)
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
	
	void ProjectWindowPopUp::OpenRenamePopUp(const std::filesystem::path& path)
	{
		if (m_openRenamePopup)
		{
			// リネームポップアップを出す
			ImGui::OpenPopup("Rename Asset");

			// 文字列を渡す
			m_targetPath = path;
			m_popupStr = m_targetPath.stem().string();

			// フラグ解除
			m_openRenamePopup = false;
		}
	}

	void ProjectWindowPopUp::OpenCreatePopUp(const std::filesystem::path& path)
	{
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
	}

	void ProjectWindowPopUp::DrawRenamePopup()
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

	void ProjectWindowPopUp::DrawCreatePopup()
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
}
