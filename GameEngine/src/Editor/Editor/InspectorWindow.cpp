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

#include "Editor/Common/ClassNameGetter.h"
#include "System/WindowManager.h"
#include "Editor/Loader/ComponentFactory.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "Managers/UI/Canvas.h"

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

	bool changed = false;

	// 全プロパティを取得
	for (auto& property : object->GetPropaties())
	{
		// 表示
		if (DrawProperty(&property)) changed = true;
	}

	// 区切り
	ImGui::NewLine();

	// GameObjectの特殊処理
	GameObject* gameObject = dynamic_cast<GameObject*>(object);

	if (!gameObject)
	{
		// Canvasの特殊処理
		Canvas* canvas = dynamic_cast<Canvas*>(object);

		if (changed) canvas->SetDrawOrder(canvas->GetDrawOrder());

		return;
	}

	ImGui::Text("ComponentList");
	ImGui::BeginChild("ComponentList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);

	// 全コンポーネントを取得
	for (auto& component : gameObject->GetAllComponents())
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
				gameObject->RemoveComponent(component);
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
					component->OnValidate();
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
	DrawAddComponent(gameObject);
}

bool InspectorWindow::DrawProperty(const Property* property)
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
			static_cast<std::string*>(property->value)
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
