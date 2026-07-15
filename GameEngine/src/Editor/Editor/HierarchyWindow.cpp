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

#include "imgui/imgui.h"

#include "System/WindowManager.h"
#include "Managers/ObjectManager.h"
#include "Managers/UI/UIManager.h"
#include "Scene/Scene.h"

#include "Editor/Loader/ObjectLoader.h"

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

		// UI
		DrawObjects(m_pScene->GetUIManager());

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
			// 親がnullなら
			if (!object->GetComponent<Transform>()->GetParent())
			{
				// 表示
				DrawGameObject(object.get());
			}
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems))
		{
			// Generateを表示
			if (ImGui::MenuItem("Generate"))
			{
				// Generate
				m_pScene->GetFactory()->Generate();
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
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				auto data = (std::wstring*)payload->Data;

				// 追加
				GameObject* obj = m_pScene->GetFactory()->Generate();

				// 読み込み
				ObjectLoader::LoadFromFile(*data, obj);
			}

			ImGui::EndDragDropTarget();
		}

		// IDをポップ
		ImGui::PopID();
	}

	void HierarchyWindow::DrawObjects(UIManager* UIManager)
	{
		// nullなら何もしない
		if (!UIManager) return;

		// IDの設定
		ImGui::PushID(UIManager);

		// タイトル
		ImGui::Text("UI");

		// スクロール領域の開始
		ImGui::BeginChild("UIManager", ImVec2(0, 0), ImGuiChildFlags_Borders);

		// 全キャンバスをループ
		for (auto& canvas : UIManager->GetAllCanvas())
		{
			// 表示
			DrawCanvas(canvas.get());
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems))
		{
			// Generateを表示
			if (ImGui::MenuItem("Generate"))
			{
				// Generate
				m_pScene->GetFactory()->GenerateCanvas();
			}
			ImGui::EndPopup();
		}

		// スクロール領域の終了
		ImGui::EndChild();

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
		std::string space = (object->HasComponent<Transform>()) ? "WORLD_OBJECT" : "UI_OBJECT";

		// 初期状態のフラグ
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		// 選択されていれば
		if (m_selected == object)
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
			m_selected = object;
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
				if (m_selected == object)
				{
					m_selected = nullptr;
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
			if (RectTransform* t = object->GetComponent<RectTransform>())
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

	void HierarchyWindow::DrawCanvas(Canvas* canvas)
	{
		// nullなら何もしない
		if (!canvas) return;

		// 名前を取得
		std::string name = canvas->GetName();

		// 空なら別名を設定
		if (name.empty())
		{
			name = "Canvas";
		}

		// 初期状態のフラグ
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		// 選択されていれば
		if (m_selected == canvas)
		{
			// 選択状態に
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		// ID衝突防止
		ImGui::PushID(canvas);

		// 拡張可能なツリーを展開
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		// ルートのトランスフォーム
		RectTransform* root = canvas->GetRootObject()->GetComponent<RectTransform>();

		// クリックされたら
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			// 選択されているオブジェクトを自分に
			m_selected = canvas;
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextItem())
		{
			// Deleteを表示
			if (ImGui::MenuItem("Delete"))
			{
				canvas->Destroy();

				// 選択中ならnullにする
				if (m_selected == canvas)
				{
					m_selected = nullptr;
				}
			}
			// Generate表示
			if (ImGui::MenuItem("Generate"))
			{
				canvas->Generate();
			}
			ImGui::EndPopup();
		}

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UI_OBJECT"))
			{
				auto data = (GameObject*)payload->Data;

				// Rectを持っていれば
				if (auto* t = data->GetComponent<RectTransform>())
				{
					t->SetParent(root->GetComponent<RectTransform>());
				}
			}
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				auto data = (std::wstring*)payload->Data;

				// 追加
				GameObject* obj = canvas->Generate();

				// 読み込み
				ObjectLoader::LoadUIFromFile(*data, obj, canvas);
			}

			ImGui::EndDragDropTarget();
		}

		// ツリーが開いてたら
		if (open)
		{
			for (auto& object : canvas->GetAllObjects())
			{
				// 直下なら
				if (object->GetComponent<RectTransform>()->GetParent() == root)
				{
					// オブジェクトの描画
					DrawGameObject(object.get());
				}
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}	// namespace REngine
