//====================================================//
// ファイル名  : HierarchyWindow.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "HierarchyWindow.h"

#include "ThirdParty/imgui/imgui.h"

#include "Common/Property/PropertyObject.h"
#include "System/WindowManager.h"
#include "Managers/ObjectManager.h"
#include "Scene/Scene.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool HierarchyWindow::DrawHierarchy()
	{
		// 描画開始
		bool clicked = StartHierarchy();

		// World
		DrawObjects(m_pScene->GetObjectManager());

		// 描画終了
		ImGui::End();

		return clicked;
	}

	bool HierarchyWindow::StartHierarchy()
	{
		// 位置とサイズを固定
		ImGui::SetNextWindowPos(ImVec2(WindowManager::Instance().GetWidthF() * (25.0f / 54), WindowManager::Instance().GetHeightF() / 12));
		ImGui::SetNextWindowSize(ImVec2(WindowManager::Instance().GetWidthF() * (1.0f / 7), WindowManager::Instance().GetHeightF() * (11.0f / 12)));

		ImGui::Begin("Hierarchy", nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		// ウィンドウへのクリックを検知
		return (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow)		// 自身のクリックと子ウィンドウのクリックを両方検知
			&& ImGui::IsMouseClicked(0));
	}

	void HierarchyWindow::DrawObjects(ObjectManager* objectManager)
	{
		// nullなら何もしない
		if (!objectManager) return;

		// IDの設定
		ImGui::PushID(objectManager);

		// タイトル
		ImGui::Text("World");

		// ウィンドウの半分をWorldに
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::BeginChild("ObjectManager", ImVec2(0, size.y / 2), ImGuiChildFlags_Borders);

		// 全オブジェクトをループ
		for (auto& object : objectManager->GetAllObject())
		{
			// Transformを持っている
			if (Transform* t = object->GetComponent<Transform>())
			{
				// 親がいなければ表示
				if (!t->GetParent()) DrawGameObject(object.get());
			}
			// RectTransformを持っている
			else if (RectTransform* t = object->GetComponent<RectTransform>())
			{
				// 親がいなければ表示
				if (!t->GetParent()) DrawGameObject(object.get());
			}
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems))
		{
			// Generateを表示
			if (ImGui::MenuItem("GenerateObject"))
			{
				// Generate
				m_pScene->GetFactory()->Generate();
			}
			// Generateを表示
			if (ImGui::MenuItem("GenerateUI"))
			{
				// Generate
				m_pScene->GetFactory()->GenerateUI();
			}
			ImGui::EndPopup();
		}

		// スクロール領域を終了
		ImGui::EndChild();

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WORLD_OBJECT"))
			{
				auto data = (GameObject*)payload->Data;

				// 自分と一致していなければ
				if (auto* t = data->GetComponent<Transform>())
				{
					t->SetParent(nullptr);
				}
			}

			ImGui::EndDragDropTarget();
		}

		// IDをポップ
		ImGui::PopID();
	}

	void HierarchyWindow::DrawGameObject(GameObject* object)
	{
		// nullなら何もしない
		if (!object) return;

		// 削除不可なら何もしない
		if (object->IsInvincible()) return;

		// 名前を取得
		std::string name = object->GetName();

		// 空なら別名を設定
		if (name.empty())
		{
			name = "GameObject";
		}

		// 所属空間を調べる
		std::string space = (object->GetComponent<Transform>()) ? "WORLD_OBJECT" : "UI_OBJECT";

		// 初期状態のフラグ
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		// 選択中オブジェクトを取得
		PropertyObject* selected = m_selected.GetSelected();

		// 選択されていれば
		if (selected == object)
		{
			// 選択状態に
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		// ID衝突防止
		ImGui::PushID(object);

		// 拡張可能なツリーを展開
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		// クリックされたら
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 選択されているオブジェクトを自分に
			m_selected.SetSelected(object);
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextItem())
		{
			// Delete
			if (ImGui::MenuItem("Delete"))
			{
				// 削除
				object->Destroy();

				// 選択中ならnullにする
				if (selected == object)
				{
					m_selected.SetSelected(nullptr);
				}
			}
			ImGui::EndPopup();
		}

		// ドラッグの開始
		if (ImGui::BeginDragDropSource())
		{
			// 渡したいデータを設定
			ImGui::SetDragDropPayload(space.c_str(), object, sizeof(*object));

			// ドラッグ中に表示される内容
			ImGui::Text(name.c_str());

			ImGui::EndDragDropSource();
		}

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(space.c_str()))
			{
				auto data = (GameObject*)payload->Data;

				// 自分と一致していなければ
				if (data != object)
				{
					if (auto* other = data->GetComponent<Transform>())
						if (auto* own = object->GetComponent<Transform>())
						{
							other->SetParent(own);
						}

					if (auto* other = data->GetComponent<RectTransform>())
						if (auto* own = object->GetComponent<RectTransform>())
						{
							other->SetParent(own);
						}
				}
			}

			ImGui::EndDragDropTarget();
		}

		// 開いていたら
		if (open)
		{
			// Transformを取得
			if (Transform* t = object->GetComponent<Transform>())
			{
				// 全子供を調べる
				for (auto* child : t->GetChildren())
				{
					// 子供を描画
					DrawGameObject(static_cast<GameObject*>(child->GetOwn()));
				}
			}
			// RectTransformを取得
			else if (RectTransform* t = object->GetComponent<RectTransform>())
			{
				// 全子供を調べる
				for (auto* child : t->GetChildren())
				{
					// 子供を描画
					DrawGameObject(static_cast<GameObject*>(child->GetOwn()));
				}
			}

			// ツリーを終了
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}	// namespace REngine
