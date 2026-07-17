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

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

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

			DirectX::SimpleMath::Quaternion* q = static_cast<DirectX::SimpleMath::Quaternion*>(property->value);

			DirectX::SimpleMath::Vector3 euler;

			// 編集中なら
			if (m_quaternionEditing)
			{
				// 状態を保持
				euler = m_quaternionCache;
			}
			else
			{
				euler = q->ToEuler();
			}

			// 表示
			bool changed = ImGui::DragFloat3(name.c_str(), &euler.x, DirectX::XM_PI / 128);

			// クリックされたフレームならキャッシュを初期化
			if (ImGui::IsItemActivated())
			{
				m_quaternionCache = q->ToEuler();
			}
			// それ以外ならキャッシュを更新
			else
			{
				m_quaternionCache = euler;
			}

			// 編集中フラグの更新
			m_quaternionEditing = ImGui::IsItemActive();

			// 変更時
			if (changed)
			{
				// 変更
				*q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);

				return true;
			}

			return false;
		}
			break;
									 // Color
		case PropertyType::Color:
			return ImGui::ColorEdit4(name.c_str(),
				&static_cast<DirectX::SimpleMath::Color*>(property->value)->x
			);

		default:
			break;
		}

		return false;
	}
	void PropertyOnInspector::End()
	{
	}
}
