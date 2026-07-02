//====================================================//
// ファイル名  : ObjectSaver.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Saver/ObjectSaver.h"

#include "GameObject/GameObject.h"
#include "Common/ClassNameGetter.h"

#include <fstream>
#include <filesystem>

//====================================================//
// 関数の実体宣言
//====================================================//

using namespace nlohmann;

nlohmann::json ObjectSaver::SaveProperty(const PropertyObject& obj)
{
	json js;

	// 全プロパティを調べる
	for (auto& property : obj.GetPropaties())
	{
		// 型によって分岐
		switch (property.type)
		{
			// int
		case PropertyType::Int:
			js[property.name] = *(static_cast<int*>(property.value));
			break;

			// float
		case PropertyType::Float:
			js[property.name] = *(static_cast<float*>(property.value));
			break;

			// bool
		case PropertyType::Bool:
			js[property.name] = *(static_cast<bool*>(property.value));
			break;

			// string
		case PropertyType::String:
			js[property.name] = *(static_cast<std::string*>(property.value));
			break;

			// Vector2
		case PropertyType::Vector2: {
			auto v = (static_cast<DirectX::SimpleMath::Vector2*>(property.value));
			js[property.name] = { v->x, v->y };
			break;
		}
			// Vector3
		case PropertyType::Vector3: {
			auto v = (static_cast<DirectX::SimpleMath::Vector3*>(property.value));
			js[property.name] = { v->x, v->y, v->z };
			break;
		}
			// Quaternion
		case PropertyType::Quaternion: {
			auto v = (static_cast<DirectX::SimpleMath::Quaternion*>(property.value));
			js[property.name] = { v->x, v->y, v->z, v->w };
			break;
		}

		default:
			break;
		}
	}

	return js;
}

json ObjectSaver::SaveObject(const GameObject* obj)
{
	json j;

	// GameObject部分を保存
	SaveProperty(*obj);

	// コンポーネントを全て調べる
	for (auto& component : obj->GetAllComponents())
	{
		// jsonを生成
		json compJson = SaveProperty(*component);

		// コンポーネント名を取得
		std::string componentName = ClassNameGetter::Get(*component);

		j["Components"].push_back(
			{
				{ "Type", componentName},
				{ "Data", compJson },
			}
		);
	}

	return j;
}

void ObjectSaver::SaveToFile(const std::wstring& filePath, GameObject* obj)
{
	std::ofstream ofs(std::filesystem::path(filePath).c_str());

	// 開けていたら
	if (ofs.is_open())
	{
		// パス
		ofs << SaveObject(obj).dump(4);
	}

	// 閉じる
	ofs.close();
}
