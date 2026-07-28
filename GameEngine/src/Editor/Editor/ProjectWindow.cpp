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
#include "System/WindowManager.h"
#include "HandlePayload.h"

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
	}

	void ProjectWindow::DrawFileStructure(const std::filesystem::path& path)
	{
		// filesystemを省略
		namespace fs = std::filesystem;

		// フォルダ名でツリーを開始
		if (ImGui::TreeNode(path.stem().string().c_str()))
		{
			// ディレクトリ直下に含まれるファイルを走査
			for (const auto& file : fs::directory_iterator(path))
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
					std::string fileName = file.path().stem().string();

					// 出力
					if (ImGui::Selectable(fileName.c_str(), false, 
						ImGuiSelectableFlags_AllowDoubleClick))	// ダブルクリック時に判定
					{
						// 左ボタンがダブルクリックされていれば
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							// 選択
							bool a = true;
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
				}
			}

			// ツリーの終了
			ImGui::TreePop();
		}
	}
}	// namespace REngine
