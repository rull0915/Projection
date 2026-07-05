//====================================================//
// ファイル名  : EditGUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Editor/EditGUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

#include "Common/ClassNameGetter.h"
#include "Loader/ComponentFactory.h"
#include "Components/World/Components.h"
#include "Components/UI/UIComponents.h"
#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EditGUI::DrawObjects(ObjectManager* objectManager)
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
	for (auto& object : objectManager->GetAllAbject())
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
			m_pScene->Generate();
		}
		ImGui::EndPopup();
	}

	// スクリール領域を終了
	ImGui::EndChild();

	// IDをポップ
	ImGui::PopID();
}

void EditGUI::DrawObjects(UIManager* UIManager)
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
			m_pScene->GenerateCanvas();
		}
		ImGui::EndPopup();
	}

	// スクロール領域の終了
	ImGui::EndChild();

	// IDをポップ
	ImGui::PopID();
}

void EditGUI::DrawImage(ID3D11ShaderResourceView* img, DirectX::SimpleMath::Vector2 position)
{
	ImGui::Image(
		(ImTextureID)img,
		{ position.x, position.y });
}

void EditGUI::DrawGameObject(GameObject* object)
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
		// Deleteを表示
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

void EditGUI::DrawCanvas(Canvas* canvas)
{
	// nullなら何もしない
	if (!canvas) return;

	// 名前を取得
	std::string name = canvas->GetRootObject()->GetName();

	// 空なら別名を設定
	if (name.empty())
	{
		name = "Canvas";
	}

	// ID衝突防止
	ImGui::PushID(canvas);

	// ルートのトランスフォーム
	RectTransform* root = canvas->GetRootObject()->GetComponent<RectTransform>();

	// ツリーを展開
	bool open = ImGui::TreeNode(name.c_str());

	// 右クリック時のメニュー
	if (ImGui::BeginPopupContextItem())
	{
		// Deleteを表示
		if (ImGui::MenuItem("Delete"))
		{
			// Todo 削除
		}
		// Generate表示
		if (ImGui::MenuItem("Generate"))
		{
			canvas->Generate();
		}
		ImGui::EndPopup();
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

void EditGUI::DrawGameObjectOnInspector(GameObject* object)
{
	// nullなら何もしない
	if (!object) return;

	ImGui::Text("GameObject");
	ImGui::Separator();

	// 全プロパティを取得
	for (auto& property : object->GetPropaties())
	{
		// 表示
		DrawProperty(&property);
	}

	// 区切り
	ImGui::NewLine();

	ImGui::Text("ComponentList");
	ImGui::BeginChild("ComponentList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);

	// 全コンポーネントを取得
	for (auto& component : object->GetAllComponents())
	{
		// コンポーネント名を取得
		std::string componentName = ClassNameGetter::Get(*component);

		// ID衝突防止
		ImGui::PushID(component);

		bool open = ImGui::TreeNode(componentName.c_str());

		// 右クリック時のメニュー
		if (ImGui::BeginPopupContextItem())
		{
			// Deleteを表示
			if (ImGui::MenuItem("Delete"))
			{
				// 削除
				object->RemoveComponent(component);
			}
			ImGui::EndPopup();
		}

		// ツリーの開始
		if (open)
		{
			// 全プロパティを取得
			for (auto& property : component->GetPropaties())
			{
				// 表示
				// 値が変わっていたら
				if (DrawProperty(&property))
				{
					OnChanged(component);
				}
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

void EditGUI::StartHierarchy()
{
	ImGui::Begin("Hierarchy");
}

void EditGUI::StartInspector()
{
	ImGui::Begin("Inspector");
}

void EditGUI::StartSceneView()
{
	ImGui::Begin("SceneView");
}

void EditGUI::StartGameView()
{
	ImGui::Begin("GameView");
}

void EditGUI::EndWindow()
{
	ImGui::End();
}

bool EditGUI::DrawProperty(const Property* property)
{
	// nullなら何もしない
	if (!property) return false;

	// 名前を取得
	std::string name = property->name;

	// 空時の例外処理
	if (name.empty()) name = "Property";

	// タイプによって分岐
	switch (property->type)
	{
		// int
	case PropertyType::Int:
		return ImGui::DragInt(name.c_str(),
			static_cast<int*>(property->value));

		// float
	case PropertyType::Float:
		return ImGui::DragFloat(name.c_str(),
			static_cast<float*>(property->value), 0.1f);

		// bool
	case PropertyType::Bool:
		return ImGui::Checkbox(name.c_str(),
			static_cast<bool*>(property->value));

		// std::string
	case PropertyType::String:
		return ImGui::InputText(name.c_str(),
			static_cast<std::string*>(property->value), 0.1f
		);
		
		// Vector2
	case PropertyType::Vector2: 
		return ImGui::DragFloat2(name.c_str(),
			&static_cast<DirectX::SimpleMath::Vector2*>(property->value)->x, 0.1f);

		// Vector3
	case PropertyType::Vector3:
		return ImGui::DragFloat3(name.c_str(),
			&static_cast<DirectX::SimpleMath::Vector3*>(property->value)->x, 0.1f);

		// Quaternion
	case PropertyType::Quaternion: {
		// 変換
		DirectX::SimpleMath::Quaternion* q = static_cast<DirectX::SimpleMath::Quaternion*>(property->value);
		// オイラー角へ
		DirectX::SimpleMath::Vector3 euler = q->ToEuler();
		// 表示
		if (ImGui::DragFloat3(name.c_str(), &euler.x, DirectX::XM_PI / 128))
		{
			// 変更
			*q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);

			return true;
		}
		return false;
	}

		// Color
	case PropertyType::Color:
		return ImGui::ColorEdit4(name.c_str(),
			&static_cast<DirectX::SimpleMath::Color*>(property->value)->x
		);

	default:
		break;
	}
}

void EditGUI::DrawAddComponent(GameObject* object)
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
		// 選択肢を表示

		// Worldコンポーネント
		if (ImGui::BeginMenu("World"))
		{
			// ファクトリから候補を取得
			for (auto& factory : ComponentFactory::m_creatorMap)
			{
				// Worldなら
				if (factory.second.first == ComponentSpace::World)
				{
					// 表示
					if (ImGui::Selectable(factory.first.c_str()))
					{
						// 押されたら追加
						factory.second.second(object);
					}
				}
			}

			ImGui::EndMenu();
		}

		// UIコンポーネント
		if (ImGui::BeginMenu("UI"))
		{
			// ファクトリから候補を取得
			for (auto& factory : ComponentFactory::m_creatorMap)
			{
				// UIなら
				if (factory.second.first == ComponentSpace::UI)
				{
					// 表示
					if (ImGui::Selectable(factory.first.c_str()))
					{
						// 押されたら追加
						factory.second.second(object);
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void EditGUI::OnChanged(ComponentBase* component)
{
	// トランスフォームなら
	if (component->GetID() == TypeIDGenerator::GetID<Transform>())
	{
		// 変更フラグをオンに
		static_cast<Transform*>(component)->SetDirty();
	}
	// レクトトランスフォームなら
	if (component->GetID() == TypeIDGenerator::GetID<RectTransform>())
	{
		// 変更フラグをオンに
		static_cast<RectTransform*>(component)->SetDirty();
	}
	// コライダーなら
	if (component->GetCategory() == Category::Collider)
	{
		// 変更フラグをオンに
		static_cast<ColliderBase*>(component)->SetDirty();
	}
	// 2Dコライダーなら
	if (component->GetCategory() == Category::Collider2D)
	{
		// 変更フラグをオンに
		static_cast<ColliderBase2D*>(component)->SetDirty();
	}
}
