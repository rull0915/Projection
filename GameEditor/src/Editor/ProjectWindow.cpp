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

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

#include "System/WindowManager.h"
#include "System/ResourceManager.h"
#include "System/PrefabManager.h"

#include "Common/OpenFileDialog.h"
#include "Saver/ObjectSaver.h"

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
	ImGui::Text("Resources");

	ImGui::BeginChild("Resources", ImVec2(0, 0), ImGuiChildFlags_Borders);

	if (ImGui::TreeNodeEx("Models", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// モデル
		for (auto& model : ResourceManager::Instance().GetAllModels())
		{
			ImGui::InputText(
				("##" + model.first).c_str(),
				const_cast<std::string*>(&model.first),
				ImGuiInputTextFlags_ReadOnly
			);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Textures", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// テクスチャ
		for (auto& texture : ResourceManager::Instance().GetAllTextures())
		{
			ImGui::InputText(
				("##" + texture.first).c_str(),
				const_cast<std::string*>(&texture.first),
				ImGuiInputTextFlags_ReadOnly
			);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Sounds", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// サウンド
		for (auto& sound : ResourceManager::Instance().GetAllSounds())
		{
			ImGui::InputText(
				("##" + sound.first).c_str(),
				const_cast<std::string*>(&sound.first),
				ImGuiInputTextFlags_ReadOnly
			);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Fonts", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// フォント
		for (auto& font : ResourceManager::Instance().GetAllFonts())
		{
			ImGui::InputText(
				("##" + font.first).c_str(),
				const_cast<std::string*>(&font.first),
				ImGuiInputTextFlags_ReadOnly
			);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// オブジェクト
		for (auto& object : PrefabManager::Instance().GetAllObjects())
		{
			ImGui::Text(object.first.c_str());	// ドラッグの開始

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				// 渡したいデータを設定
				ImGui::SetDragDropPayload("PREFAB", &object.second, sizeof(object.second));

				// ドラッグ中に表示される内容
				ImGui::Text(object.first.c_str());

				ImGui::EndDragDropSource();
			}
		}

		ImGui::TreePop();
	}

	ImGui::EndChild();


	if (ImGui::BeginDragDropTarget())
	{
		// WORLDオブジェクトを受け取ったら
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WORLD_OBJECT"))
		{
			auto data = (GameObject*)payload->Data;

			// ファイルを開き保存
			auto path = FileDialog::Open(FileDialog::Mode::Save, L"Resources/Objects/", L".gameobject");

			// セーブ
			if (!path.empty())
			{
				ObjectSaver::SaveObjectToFile(
					path,
					data
				);

				// マネージャーに追加
				PrefabManager::Instance().AddPrefab(std::filesystem::path(path).stem().string(), path);
			}
		}
		ImGui::EndDragDropTarget();
	}
}
