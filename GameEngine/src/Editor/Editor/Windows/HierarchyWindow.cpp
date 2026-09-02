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

		// ウィンドウの半分をWorldに
		ImGui::BeginChild("ObjectManager", ImVec2(0, 0), ImGuiChildFlags_Borders);

		// 全オブジェクトをループ
		for (auto& object : objectManager->GetAllObject())
		{
			// 親がいなければ表示
			if (!object->GetParent()) DrawGameObject(object.get());
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
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				auto data = (GameObject*)payload->Data;

				// Transformを持っていれば
				if (auto* t = data->GetComponent<Transform>())
				{
					t->SetParent(nullptr);
				}
				// RectTransformを持っていれば
				if (auto* t = data->GetComponent<RectTransform>())
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
		
		// 子供がいなければ▼を表示しない
		if (object->GetChildCount() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// 拡張可能なツリーを展開
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		// クリックされたら
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
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
			// Generate
			if (ImGui::MenuItem("Generate"))
			{
				// World空間オブジェクトの場合
				if (Transform* parent = object->GetComponent<Transform>())
				{
					// 生成
					GameObject* child = m_pScene->GetFactory()->Generate();

					// 親を設定
					child->GetComponent<Transform>()->SetParent(parent);

					// ローカル情報をリセット
					child->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3::Zero);
					child->GetComponent<Transform>()->SetLocalRotation(DirectX::SimpleMath::Quaternion::Identity);
					child->GetComponent<Transform>()->SetLocalScale({ 1, 1, 1 });
				}

				// UI空間オブジェクトの場合
				if (RectTransform* parent = object->GetComponent<RectTransform>())
				{
					// 生成
					GameObject* child = m_pScene->GetFactory()->GenerateUI();

					// 親を設定
					child->GetComponent<RectTransform>()->SetParent(parent);

					// ローカル情報をリセット
					child->GetComponent<RectTransform>()->SetAnchoredPosition(DirectX::SimpleMath::Vector2::Zero);
					child->GetComponent<RectTransform>()->SetRotation(0);
					child->GetComponent<RectTransform>()->SetScale({ 1, 1 });
				}
			}
			ImGui::EndPopup();
		}

		// ドラッグの開始
		if (ImGui::BeginDragDropSource())
		{
			// 渡したいデータを設定
			ImGui::SetDragDropPayload("GAMEOBJECT", object, sizeof(*object));

			// ドラッグ中に表示される内容
			ImGui::Text(name.c_str());

			ImGui::EndDragDropSource();
		}

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				auto data = (GameObject*)payload->Data;

				// 自分と一致していなければ
				if (data != object)
				{
					// Transformの時
					if (auto* other = data->GetComponent<Transform>())
						if (auto* own = object->GetComponent<Transform>())
						{
							other->SetParent(own);
						}

					// RectTransformの時
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
