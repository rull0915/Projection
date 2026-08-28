//====================================================//
// ファイル名  : InspectorWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : インスペクターウィンドウ
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "InspectorWindow.h"

#include "ThirdParty/nameof/nameof.hpp"
#include "System/WindowManager.h"
#include "Editor/Loader/ComponentFactory.h"

#include "ThirdParty/imgui/imgui.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool InspectorWindow::DrawInspector()
	{
		// 描画開始
		bool clicked = StartInspector();

		// 選択中オブジェクトを取得
		PropertyObject* selected = m_selected.GetSelected();

		// 選択されていたら
		if (selected)
		{
			DrawPropertyObjectOnInspector(selected);
		}

		// 描画終了
		ImGui::End();

		return clicked;
	}

	bool InspectorWindow::StartInspector()
	{
		// 位置とサイズを固定
		ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (3.0f / 4), WindowManager::Instance().GetHeightF() / 12));
		ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (1.0f / 4), WindowManager::Instance().GetHeightF() * (11.0f / 12)));

		ImGui::Begin("Inspector", nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		// ウィンドウへのクリックを検知
		return (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
			&& ImGui::IsMouseClicked(0));
	}

	void InspectorWindow::DrawPropertyObjectOnInspector(PropertyObject* object)
	{
		// nullなら何もしない
		if (!object) return;

		// GameObjectの場合の表示
		if (GameObject* gameObject = dynamic_cast<GameObject*>(object))
		{
			DrawGameObject(gameObject);
		}
		// Assetの場合の表示
		else if (AssetBase* asset = dynamic_cast<AssetBase*>(object))
		{
			DrawAsset(asset);
		}
	}

	void InspectorWindow::DrawGameObject(GameObject* object)
	{
		// UUIDを表示(テスト)
		ImGui::Text("UUID : %d", object->GetUUID());

		// 表示
		if (m_propertyOnInspector.DrawPropertyObject(object))
		{
			object->OnValidate();
		}

		// 全コンポーネントを取得
		for (auto& component : object->GetAllComponents())
		{
			// コンポーネント名を取得
			auto name = NAMEOF_SHORT_TYPE_RTTI(*component);

			// ID衝突防止
			ImGui::PushID(component);

			bool open = ImGui::TreeNode(name.data());

			// 右クリック時のメニュー
			if (ImGui::BeginPopupContextItem())
			{
				// Deleteを表示
				if (ImGui::MenuItem("Delete"))
				{
					// 押されたら削除
					object->RemoveComponent(component);
				}
				ImGui::EndPopup();
			}

			// ツリーの開始
			if (open)
			{
				// UUIDを表示(テスト)
				ImGui::Text("UUID : %d", component->GetUUID());

				// インスペクターに表示
				if (m_propertyOnInspector.DrawPropertyObject(component))
				{
					// 変更時関数の呼び出し
					component->OnValidate();
				}

				// 区切り
				ImGui::Separator();

				// ツリーの終了
				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		// AddComponentボタンを追加
		DrawAddComponent(object);
	}

	void InspectorWindow::DrawAddComponent(GameObject* object)
	{
		// nullなら何もしない
		if (!object) return;

		// ボタンを表示

		// 押されたら
		if (ImGui::Button("Add Component"))
		{
			// AddComponentのポップアップを開く
			ImGui::OpenPopup("AddComponent");
		}

		// プロジェクトタイプの一覧
		constexpr std::pair<const char*, ComponentProject> projects[] =
		{
			{ "Standard", ComponentProject::Engine },
			{ "Original", ComponentProject::Game },
		};

		// コンポーネント所属空間の一覧
		constexpr std::pair<const char*, ComponentSpace> spaces[] =
		{
			{ "Both",  ComponentSpace::Both  },
			{ "World", ComponentSpace::World },
			{ "UI",    ComponentSpace::UI    },
		};

		// AddCompnentが開かれていたら
		if (ImGui::BeginPopup("AddComponent"))
		{
			// プロジェクトをループ
			for (auto& [pStr, project] : projects)
			{
				// 開かれていれば
				if (ImGui::BeginMenu(pStr))
				{
					// 所属空間をループ
					for (auto& [sStr, space] : spaces)
					{
						// 開かれていれば
						if (ImGui::BeginMenu(sStr))
						{
							// コンポーネントリストを表示
							DrawComponentList(object, { project, space });

							ImGui::EndMenu();
						}
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndPopup();
		}
	}

	void InspectorWindow::DrawComponentList(GameObject* object, ComponentInfo info)
	{
		// ファクトリから候補を取得
		for (auto& factory : ComponentFactory::m_creatorMap)
		{
			// 一致したら
			if (factory.second.first.project == info.project && factory.second.first.space == info.space)
			{
				// 表示
				if (ImGui::Selectable(factory.first.c_str()))
				{
					// 押されたら追加
					factory.second.second(object);
				}
			}
		}
	}

	void InspectorWindow::DrawAsset(AssetBase* asset)
	{
		// Handleを取得
		UnTypeHandle* handle = m_selected.GetSelectedHandle();

		// Handleからパスへ
		auto& path = m_assetManager.GetDataBase().GetPath(m_assetManager.GetUUID(*handle));

		// fs::pathに変換しファイル名を取得
		std::string stem = std::filesystem::path(path).stem().string();

		ImGui::Text(stem.c_str());

		// 本体を表示
		m_propertyOnInspector.DrawPropertyObject(asset);

		// セーブ可能かどうか
		if (m_assetManager.CanSave(path))
		{
			// ボタンを表示
			if (ImGui::Button("Save"))
			{
				// 保存
				m_assetManager.SaveAsset(path);
			}
		}
	}
}	// namespace REngine
