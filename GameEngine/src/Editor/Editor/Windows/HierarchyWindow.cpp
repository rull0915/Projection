/**
 * @file    HierarchyWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "HierarchyWindow.h"

#include "ThirdParty/imgui/imgui.h"

#include "Common/Property/PropertyObject.h"
#include "Managers/ObjectManager.h"
#include "Scene/Scene.h"

#include "Assets/Managers/AssetManager.h"
#include "Editor/Editor/HandlePayload.h"
#include "Assets/Types/Prefab.h"

#include "Scene/ObjectFactory.h"

namespace RHEngine
{
	void HierarchyWindow::DrawHierarchy()
	{
		// 描画開始
		StartHierarchy();

		// World
		DrawObjects(m_pScene->GetObjectManager());

		// 描画終了
		ImGui::End();
	}

	void HierarchyWindow::StartHierarchy()
	{
		ImGui::Begin("Hierarchy", nullptr);
	}

	void HierarchyWindow::DrawObjects(ObjectManager* pObjectManager)
	{
		// nullなら何もしない
		if (!pObjectManager) return;

		// IDの設定
		ImGui::PushID(pObjectManager);

		// ウィンドウの半分をWorldに
		ImGui::BeginChild("ObjectManager", ImVec2(0, 0), ImGuiChildFlags_Borders);

		// 全オブジェクトをループ
		for (auto& object : pObjectManager->GetAllObject())
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
			if (const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				auto pData = (GameObject*)pPayload->Data;

				// Transformを持っていれば
				if (auto* t = pData->GetComponent<Transform>())
				{
					t->SetParent(nullptr);
				}
				// RectTransformを持っていれば
				if (auto* t = pData->GetComponent<RectTransform>())
				{
					t->SetParent(nullptr);
				}
			}

			else if (const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("ASSET"))
			{
				// 受け渡し構造体を生成
				HandlePayload* pHandlePay = (HandlePayload*)pPayload->Data;

				// Prefabの時
				if (pHandlePay && pHandlePay->type == std::type_index(typeid(Prefab)))
				{
					// プレハブハンドルを取得
					auto handle = pHandlePay->handle.As<Prefab>();

					// 生成
					if (m_pScene) m_pScene->GetFactory()->Instantiate(m_pScene->GetAssetManager().Get(handle));
				}
			}

			ImGui::EndDragDropTarget();
		}

		// IDをポップ
		ImGui::PopID();
	}

	bool HierarchyWindow::SelectCancel(GameObject* root)
	{
		// 選択中のオブジェクトを取得
		PropertyObject* pSelected = m_selected.GetSelected();

		// もし自身が選択されていればリセット
		if (root == pSelected)
		{
			m_selected.SetSelected(nullptr);

			return true;
		}

		// 子オブジェクトも調べる
		for (auto pChild : root->GetChildren())
		{
			if (SelectCancel(pChild)) return true;
		}

		return false;
	}

	void HierarchyWindow::DrawGameObject(GameObject* pObject)
	{
		// nullなら何もしない
		if (!pObject) return;

		// シリアライズしない設定なら何もしない
		if (!pObject->IsSerializable()) return;

		// 名前を取得
		std::string name = pObject->GetName();

		// 空なら別名を設定
		if (name.empty())
		{
			name = "GameObject";
		}

		// 初期状態のフラグ
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

		// 選択中オブジェクトを取得
		PropertyObject* pSelected = m_selected.GetSelected();

		// 選択されていれば
		if (pSelected == pObject)
		{
			// 選択状態に
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		// ID衝突防止
		ImGui::PushID(pObject);
		
		// 子供がいなければ▼を表示しない
		if (pObject->GetChildCount() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		// 拡張可能なツリーを展開
		bool open = ImGui::TreeNodeEx(name.c_str(), flags);

		// クリックされたら
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// 選択されているオブジェクトを自分に
			m_selected.SetSelected(pObject);
		}

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextItem())
		{
			// Delete
			if (ImGui::MenuItem("Delete"))
			{
				// 削除
				pObject->Destroy();

				// 選択解除
				SelectCancel(pObject);
			}
			// Generate
			if (ImGui::MenuItem("Generate"))
			{
				// World空間オブジェクトの場合
				if (Transform* pParent = pObject->GetComponent<Transform>())
				{
					// 生成
					GameObject* pChild = m_pScene->GetFactory()->Generate();

					// 親を設定
					pChild->GetComponent<Transform>()->SetParent(pParent);

					// ローカル情報をリセット
					pChild->GetComponent<Transform>()->SetLocalPosition(DirectX::SimpleMath::Vector3::Zero);
					pChild->GetComponent<Transform>()->SetLocalRotation(DirectX::SimpleMath::Quaternion::Identity);
					pChild->GetComponent<Transform>()->SetLocalScale({ 1, 1, 1 });
				}

				// UI空間オブジェクトの場合
				if (RectTransform* pParent = pObject->GetComponent<RectTransform>())
				{
					// 生成
					GameObject* pChild = m_pScene->GetFactory()->GenerateUI();

					// 親を設定
					pChild->GetComponent<RectTransform>()->SetParent(pParent);

					// ローカル情報をリセット
					pChild->GetComponent<RectTransform>()->SetAnchoredPosition(DirectX::SimpleMath::Vector2::Zero);
					pChild->GetComponent<RectTransform>()->SetRotation(0);
					pChild->GetComponent<RectTransform>()->SetScale({ 1, 1 });
				}
			}
			ImGui::EndPopup();
		}

		// ドラッグの開始
		if (ImGui::BeginDragDropSource())
		{
			// 渡したいデータを設定
			ImGui::SetDragDropPayload("GAMEOBJECT", pObject, sizeof(*pObject));

			// ドラッグ中に表示される内容
			ImGui::Text(name.c_str());

			ImGui::EndDragDropSource();
		}

		// ドラッグの受け取り
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				auto pData = (GameObject*)pPayload->Data;

				// 自分と一致していなければ
				if (pData != pObject)
				{
					// Transformの時
					if (auto* pOther = pData->GetComponent<Transform>())
						if (auto* pOwn = pObject->GetComponent<Transform>())
						{
							pOther->SetParent(pOwn);
						}

					// RectTransformの時
					if (auto* pOther = pData->GetComponent<RectTransform>())
						if (auto* pOwn = pObject->GetComponent<RectTransform>())
						{
							pOther->SetParent(pOwn);
						}
				}
			}

			ImGui::EndDragDropTarget();
		}

		// 開いていたら
		if (open)
		{
			// 全子供を調べる
			for (auto* pChild : pObject->GetChildren())
			{
				// 子供を描画
				DrawGameObject(pChild);
			}

			// ツリーを終了
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
