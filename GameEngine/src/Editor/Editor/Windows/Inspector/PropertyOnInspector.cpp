//====================================================//
// ファイル名  : PropertyOnInspector.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/17
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"

#ifdef ENGINE_GUI

#include "PropertyOnInspector.h"
#include "Assets/Managers/AssetManager.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_stdlib.h"

#include "Editor/Editor/HandlePayload.h"
#include "Common/ObjectReference.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool PropertyOnInspector::DrawPropertyObject(PropertyObject* object, bool readOnly)
	{
		// 変更フラグ
		bool changed = false;

		ImGui::BeginDisabled(readOnly);

		// 全プロパティを表示
		for(auto& p : object->GetProperties())
		{
			if (DrawProperty(&p)) changed = true;
		}

		ImGui::EndDisabled();

		return changed;
	}

	bool PropertyOnInspector::DrawProperty(Property* property)
	{
		// nullなら何もしない
		if (!property) return false;

		// 名前を取得
		std::string name = property->name;

		// 空時の例外処理
		if (name.empty()) name = "Property";

		// 変更フラグ
		bool changed = false;

		ImGui::PushID(property->value);

		// タイプによって分岐
		switch (property->type)
		{
			// int
		case PropertyType::Int:
			changed = ImGui::DragInt(name.c_str(), static_cast<int*>(property->value));
			break;

			// float
		case PropertyType::Float:
			changed = ImGui::DragFloat(name.c_str(), static_cast<float*>(property->value), 0.1f);
			break;

			// bool
		case PropertyType::Bool:
			changed = ImGui::Checkbox(name.c_str(), static_cast<bool*>(property->value));
			break;

			// std::string
		case PropertyType::String:
			changed = ImGui::InputText(name.c_str(), static_cast<std::string*>(property->value)
			);
			break;

			// Vector2
		case PropertyType::Vector2:
			changed = ImGui::DragFloat2(name.c_str(), &static_cast<DirectX::SimpleMath::Vector2*>(property->value)->x, 0.1f);
			break;

			// Vector3
		case PropertyType::Vector3:
			changed = ImGui::DragFloat3(name.c_str(), &static_cast<DirectX::SimpleMath::Vector3*>(property->value)->x, 0.1f);
			break;

			// Vector4
		case PropertyType::Vector4:
			changed = ImGui::DragFloat4(name.c_str(), &static_cast<DirectX::SimpleMath::Vector4*>(property->value)->x, 0.1f);
			break;

			// Quaternion 
		case PropertyType::Quaternion: 
			changed = DrawQuaternion(property);
			break;

			// Color
		case PropertyType::Color:
			changed = ImGui::ColorEdit4(name.c_str(), &static_cast<DirectX::SimpleMath::Color*>(property->value)->x);
			break;

			// PropertyObject派生
		case PropertyType::Object:
			changed = DrawObject(property);
			break;

			// 列挙型
		case PropertyType::Enum: 
			changed = DrawEnum(property);
			break;

			// AssetHandle
		case PropertyType::AssetHandle: 
			changed = DrawAssetHandle(property);
			break;

			// 参照型
		case PropertyType::ObjectRef:		
			changed = DrawReference(property);
			break;

			// 配列
		case PropertyType::Array:
			changed = DrawArray(property);
			break;

		case PropertyType::Header:
			ImGui::Spacing();
			ImGui::SeparatorText(property->name.c_str());
			break;

		default:
			break;
		}

		ImGui::PopID();

		return changed;
	}

	bool PropertyOnInspector::DrawQuaternion(Property* property)
	{
		bool changed = false;

		// クォータニオンポインタに変換
		DirectX::SimpleMath::Quaternion* q = static_cast<DirectX::SimpleMath::Quaternion*>(property->value);

		// キャッシュされているオイラー角を取得
		DirectX::SimpleMath::Vector3 euler = m_quaternionCache;

		// 編集中でなければ新たにオイラー角を生成
		if (!m_quaternionEditing) euler = q->ToEuler();

		// 表示
		changed = ImGui::DragFloat3(property->name.c_str(), &euler.x, DirectX::XM_PI / 128);

		// クリックされたフレームならキャッシュを初期化
		if (ImGui::IsItemActivated()) m_quaternionCache = q->ToEuler();

		// それ以外ならキャッシュを更新
		else m_quaternionCache = euler;

		// 編集中フラグの更新
		m_quaternionEditing = ImGui::IsItemActive();

		// 変更時
		if (changed)
		{
			// 変更
			*q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
		}

		return changed;
	}

	bool PropertyOnInspector::DrawObject(Property* property)
	{
		bool changed = false;

		// ツリーの開始
		if (ImGui::TreeNode(property->name.c_str()))
		{
			// 表示
			changed = DrawPropertyObject(static_cast<PropertyObject*>(property->value));

			// ツリーの終了
			ImGui::TreePop();
		}

		return changed;
	}

	bool PropertyOnInspector::DrawEnum(Property* property)
	{
		bool changed = false;

		// 列挙型管理クラスを取得
		auto& registry = EnumRegistry::Instance();

		// 列挙子の文字列配列を取得
		std::vector<std::string> names = registry.GetNames(property->typeIndex);

		// 現在の列挙子名を取得
		std::string currentName = registry.GetCurrentName(property->typeIndex, property->value);

		// コンボボックス表示
		if (ImGui::BeginCombo(property->name.c_str(), currentName.c_str()))
		{
			// 開かれたら全列挙子を表示
			for (auto& name : names)
			{
				// 選択中かどうか
				bool selected = (name == currentName);

				// 選択肢を表示
				if (ImGui::Selectable(name.c_str(), selected))
				{
					// 選ばれたらセット
					registry.SetByName(property->typeIndex, property->value, name);

					// フラグをオン
					changed = true;
				}

				// 開かれたときのデフォルトを自分に指定
				if (selected) ImGui::SetItemDefaultFocus();
			}

			// コンボボックスを終了
			ImGui::EndCombo();
		}

		return changed;
	}

	bool PropertyOnInspector::DrawAssetHandle(Property* property)
	{
		bool changed = false;

		// UUIDを取得
		UUID uuid = AssetPropertyRegistry::Instance().GetUUID(property->typeIndex, property->value, m_assetManager);

		// UUIDから名前を取得
		std::string name = uuid == 0 ? "" : std::filesystem::path(m_assetManager.GetDataBase().GetPath(uuid)).stem().string();

		ImGui::Text(property->name.c_str());

		ImGui::SameLine();

		// 表示
		ImGui::Selectable(name.c_str(), false);

		// 描画リストを取得
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Rectを追加
		drawList->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 64));

		// ドラッグを受け取る
		if (ImGui::BeginDragDropTarget())
		{
			// ASSETがドロップされたら
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
			{
				// HandlePayloadに変換
				HandlePayload* pay = static_cast<HandlePayload*>(payload->Data);

				// type_indexが一致しなければ何もしない
				if (pay->type == property->typeIndex)
				{
					// ハンドルを変更する
					AssetPropertyRegistry::Instance().Assign(property->typeIndex, property->value, pay->handle);

					changed = true;
				}
			}

			ImGui::EndDragDropTarget();
		}

		// リセットボタン
		if (ImGui::Button("Reset"))
		{
			// ハンドルを変更する
			AssetPropertyRegistry::Instance().Assign(property->typeIndex, property->value, ERROR_UNTYPE_HANDLE);

			changed = true;
		}

		return changed;
	}

	bool PropertyOnInspector::DrawReference(Property* property)
	{
		bool changed = false;

		// RefBaseに変換
		RefBase* refBase = static_cast<RefBase*>(property->value);

		// 変数名を表示
		ImGui::Text(property->name.c_str());

		// 同じライン
		ImGui::SameLine();

		// 表示

		// ObjRefから名前を取得
		PropertyObject* refObj = refBase->GetPropertyObject();

		std::string name = "";

		// 設定されているとき
		if (refObj)
		{
			if (GameObject* gameObj = dynamic_cast<GameObject*>(refObj))
			{
				name = gameObj->GetName();
			}
			else if (ComponentBase* compObj = dynamic_cast<ComponentBase*>(refObj))
			{
				name = compObj->GetOwn()->GetName();
			}
		}

		ImGui::Selectable(name.c_str(), false);

		// 描画リストを取得
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Rectを追加
		drawList->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 64));

		// ドラッグを受け取る
		if (ImGui::BeginDragDropTarget())
		{
			// GAMEOBJECTがドロップされたら
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				// GameObjectに変換
				GameObject* obj = static_cast<GameObject*>(payload->Data);

				// UUIDをセット
				refBase->SetUUID(obj->GetUUID());

				// シーン経由で参照解決
				changed = m_pScene->ResolveRef(refBase);

				// 解決失敗した場合
				if (!changed)
				{
					// コンポーネントをチェック
					for (auto& component : obj->GetAllComponents())
					{
						// UUIDをセット
						refBase->SetUUID(component->GetUUID());

						// シーン経由で参照解決
						// 成功すればその時点で終了
						if (changed = m_pScene->ResolveRef(refBase)) break;
					}
				}
			}

			// COMPONENTがドロップされたら
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT"))
			{
				// Componentに変換
				ComponentBase* obj = static_cast<ComponentBase*>(payload->Data);

				// UUIDをセット
				refBase->SetUUID(obj->GetUUID());

				// シーン経由で参照解決
				changed = m_pScene->ResolveRef(refBase);
			}

			ImGui::EndDragDropTarget();
		}

		return changed;
	}

	bool PropertyOnInspector::DrawArray(Property* property)
	{
		bool changed = false;

		// 配列型管理クラスを取得
		auto& registry = ArrayRegistry::Instance();

		std::type_index idx = property->typeIndex;
		void* value = property->value;

		// ツリーの開始
		if (ImGui::TreeNode(property->name.c_str()))
		{
			// サイズ分ループ
			for (size_t i = 0; i < registry.GetSize(idx, value); ++i)
			{
				// 一時Propertyを作成
				Property tempProperty = registry.GetProperty(idx, value, i);

				// 描画
				bool c = DrawProperty(&tempProperty);

				// 変更フラグ更新
				if (c) changed = true;
			}

			// 追加ボタン
			if (ImGui::Button(" + "))
			{
				// 追加
				registry.AddElement(idx, value);
			}

			ImGui::SameLine();

			// 削除ボタン
			if (ImGui::Button(" － "))
			{
				// 削除
				registry.PopBackElement(idx, value);
			}

			// ツリーの終了
			ImGui::TreePop();
		}

		return changed;
	}
}

#endif // ENGINE_GUI
