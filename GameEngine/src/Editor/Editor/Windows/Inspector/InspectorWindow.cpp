/**
 * @file    InspectorWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "InspectorWindow.h"

#include "ThirdParty/nameof/nameof.hpp"
#include "Editor/Loader/ComponentFactory.h"

#include "ThirdParty/imgui/imgui.h"

namespace RHEngine
{
	void InspectorWindow::DrawInspector()
	{
		// 描画開始
		StartInspector();

		// 選択中オブジェクトを取得
		PropertyObject* pSelected = m_selected.GetSelected();

		// 選択されていたら
		if (pSelected)
		{
			DrawPropertyObjectOnInspector(pSelected);
		}

		// 描画終了
		ImGui::End();
	}

	void InspectorWindow::StartInspector()
	{
		ImGui::Begin("Inspector", nullptr);
	}

	void InspectorWindow::DrawPropertyObjectOnInspector(PropertyObject* pObject)
	{
		// nullなら何もしない
		if (!pObject) return;

		// GameObjectの場合の表示
		if (GameObject* pGameObject = dynamic_cast<GameObject*>(pObject))
		{
			DrawGameObject(pGameObject);
		}
		// Assetの場合の表示
		else if (AssetBase* pAsset = dynamic_cast<AssetBase*>(pObject))
		{
			DrawAsset(pAsset);
		}
	}

	void InspectorWindow::DrawGameObject(GameObject* pObject)
	{
		// 表示
		if (m_propertyOnInspector.DrawPropertyObject(pObject))
		{
			pObject->OnValidate();
		}

		// 全コンポーネントを取得
		for (auto& pComponent : pObject->GetAllComponents())
		{
			// コンポーネント名を取得
			auto name = NAMEOF_SHORT_TYPE_RTTI(*pComponent);

			// ID衝突防止
			ImGui::PushID(pComponent);

			bool open = ImGui::TreeNodeEx(name.data(), ImGuiTreeNodeFlags_DefaultOpen);

			// 右クリック時のメニュー
			if (ImGui::BeginPopupContextItem())
			{
				// Deleteを表示
				if (ImGui::MenuItem("Delete"))
				{
					// 押されたら削除
					pObject->RemoveComponent(pComponent);
				}
				ImGui::EndPopup();
			}

			// ドラッグの開始
			if (ImGui::BeginDragDropSource())
			{
				// 渡したいデータを設定
				ImGui::SetDragDropPayload("COMPONENT", pComponent, sizeof(*pComponent));

				// ドラッグ中に表示される内容
				ImGui::Text(name.c_str());

				ImGui::EndDragDropSource();
			}

			// ツリーの開始
			if (open)
			{
				// インスペクターに表示
				if (m_propertyOnInspector.DrawPropertyObject(pComponent))
				{
					// 変更時関数の呼び出し
					pComponent->OnValidate();
				}

				// 区切り
				ImGui::Separator();

				// ツリーの終了
				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		// AddComponentボタンを追加
		DrawAddComponent(pObject);
	}

	void InspectorWindow::DrawAddComponent(GameObject* pObject)
	{
		// nullなら何もしない
		if (!pObject) return;

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
							DrawComponentList(pObject, { project, space });

							ImGui::EndMenu();
						}
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndPopup();
		}
	}

	void InspectorWindow::DrawComponentList(GameObject* pObject, ComponentInfo info)
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
					factory.second.second(pObject);
				}
			}
		}
	}

	void InspectorWindow::DrawAsset(AssetBase* pAsset)
	{
		// fs::pathに変換しファイル名を取得
		std::string name = pAsset->GetName();

		ImGui::Text(name.c_str());

		// 本体を表示
		if (m_propertyOnInspector.DrawPropertyObject(pAsset, pAsset->IsReadOnly()))
		{
			pAsset->OnValidate();
		}

		// セーブ可能かどうか
		auto& path = m_assetManager.GetDataBase().GetPath(pAsset->GetUUID());

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
}	// namespace RHEngine

#endif // ENGINE_GUI
