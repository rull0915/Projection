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
#include "PropertyOnInspector.h"

#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_stdlib.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	bool PropertyOnInspector::DrawPropertyObject(const PropertyObject* object)
	{
		// 変更フラグ
		bool changed = false;

		// 全プロパティを表示
		for(auto& p : object->GetPropaties())
		{
			if (DrawProperty(&p)) changed = true;
		}

		return changed;
	}

	bool PropertyOnInspector::DrawProperty(const Property* property)
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

			// ジンバルロックを防ぐための処理
			DirectX::SimpleMath::Quaternion* q = static_cast<DirectX::SimpleMath::Quaternion*>(property->value);

			DirectX::SimpleMath::Vector3 euler;

			// 編集中なら状態を保持
			if (m_quaternionEditing) euler = m_quaternionCache;

			// そうでなければオイラー角を生成
			else euler = q->ToEuler();

			// 表示
			bool changed = ImGui::DragFloat3(name.c_str(), &euler.x, DirectX::XM_PI / 128);

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
			break;

			// Color
		case PropertyType::Color:
			return ImGui::ColorEdit4(name.c_str(),
				&static_cast<DirectX::SimpleMath::Color*>(property->value)->x
			);

			// PropertyObject派生
		case PropertyType::Object: {

			bool changed = false;
			// ツリーの開始
			if (ImGui::TreeNode(name.data()))
			{
				// 表示
				changed = DrawPropertyObject(static_cast<PropertyObject*>(property->value));

				// ツリーの終了
				ImGui::TreePop();
			}
			return changed;
		}

			// 列挙型
		case PropertyType::Enum: {

			// 変更フラグ
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

		default:
			break;
		}

		return false;
	}
}
