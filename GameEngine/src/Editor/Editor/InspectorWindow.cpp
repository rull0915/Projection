//====================================================//
// ファイル名  : InspectorWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
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
#include "Managers/UI/Canvas.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool InspectorWindow::DrawInspector(PropertyObject* selected)
	{
		// 描画開始
		bool clicked = StartInspector();

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

		ImGui::Text("Object");
		ImGui::Separator();

		// 表示
		bool changed = m_propertyOnInspector.DrawPropertyObject(object);

		// 区切り
		ImGui::NewLine();

		// GameObjectの特殊処理
		if (GameObject* gameObject = dynamic_cast<GameObject*>(object))
		{
			// 変更時関数の呼び出し
			if (changed) gameObject->OnValidate();

			// 表示
			DrawGameObject(gameObject);
		}
		else if(Canvas* canvas = dynamic_cast<Canvas*>(object))
		{			
			// 描画順変更の適用
			if (changed) canvas->SetDrawOrder(canvas->GetDrawOrder());
		}
	}

	void InspectorWindow::DrawGameObject(GameObject* object)
	{
		ImGui::Text("ComponentList");
		ImGui::BeginChild("ComponentList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);

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

		ImGui::EndChild();

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

		// AddCompnentが開かれていたら
		if (ImGui::BeginPopup("AddComponent"))
		{
			// Engine側が開かれていれば
			if (ImGui::BeginMenu("Standard"))
			{
				// Bothが開かれていれば
				if (ImGui::BeginMenu("Both"))
				{
					// コンポーネントリストを表示
					DrawComponentList(object, { ComponentProject::Engine, ComponentSpace::Both });

					//終了
					ImGui::EndMenu();
				}
				// Worldが開かれていれば
				if (ImGui::BeginMenu("World"))
				{
					// コンポーネントリストを表示
					DrawComponentList(object, { ComponentProject::Engine, ComponentSpace::World });

					//終了
					ImGui::EndMenu();
				}
				// UIが開かれていれば
				if (ImGui::BeginMenu("UI"))
				{
					DrawComponentList(object, { ComponentProject::Engine, ComponentSpace::UI });

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			// オリジナルが開かれていれば
			if (ImGui::BeginMenu("Original"))
			{
				// Bothが開かれていれば
				if (ImGui::BeginMenu("Both"))
				{
					// コンポーネントリストを表示
					DrawComponentList(object, { ComponentProject::Game, ComponentSpace::Both });

					//終了
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("World"))
				{
					DrawComponentList(object, { ComponentProject::Game, ComponentSpace::World });

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("UI"))
				{
					DrawComponentList(object, { ComponentProject::Game, ComponentSpace::UI });

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
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
}	// namespace REngine
