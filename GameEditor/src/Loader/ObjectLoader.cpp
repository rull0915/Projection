//====================================================//
// ファイル名  : ObjectLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/30
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include <fstream>

#include "Loader/ObjectLoader.h"
#include "Loader/ComponentFactory.h"

#include "GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ObjectLoader::LoadProperty(const nlohmann::json& json, PropertyObject& obj)
{
	// 登録されているプロパティを全て調べる
	for (auto& property : obj.GetPropaties())
	{
		// 型によって分岐
		switch (property.type)
		{
			// int
		case PropertyType::Int:
			*(static_cast<int*>(property.value)) = json[property.name];
			break;

			// float
		case PropertyType::Float:
			*(static_cast<float*>(property.value)) = json[property.name];
			break;

			// bool
		case PropertyType::Bool:
			*(static_cast<bool*>(property.value)) = json[property.name];
			break;

			// string
		case PropertyType::String:
			*(static_cast<std::string*>(property.value)) = json[property.name];
			break;

			// Vector2
		case PropertyType::Vector2:
			*(static_cast<DirectX::SimpleMath::Vector2*>(property.value)) = { json[property.name][0], json[property.name][1] };
			break;

			// Vector3
		case PropertyType::Vector3:
			*(static_cast<DirectX::SimpleMath::Vector3*>(property.value)) = { json[property.name][0], json[property.name][1], json[property.name][2] };
			break;

			// Quaternion
		case PropertyType::Quaternion:
			*(static_cast<DirectX::SimpleMath::Quaternion*>(property.value)) = { json[property.name][0], json[property.name][1], json[property.name][2], json[property.name][3] };
			break;

		default:
			break;
		}
	}
}

void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* obj)
{
	// ゲームオブジェクト部分をロード
	LoadProperty(json, *obj);

	// コンポーネントをロード
	for (auto& js : json["Components"])
	{
		// 生成
		ComponentBase* component = ComponentFactory::Create(js["Type"], obj);

		// ロード
		if (component) LoadProperty(js["Data"], *component);
	}
}

void ObjectLoader::LoadFromFile(const std::wstring& filePath, GameObject* obj)
{
	std::ifstream ifs(std::filesystem::path(filePath).c_str());

	// 開けていたら
	if (ifs.is_open())
	{
		// jsonから読み取り
		nlohmann::json j;
		ifs >> j;

		// ロード
		LoadObject(j, obj);
	}

	// 閉じる
	ifs.close();
}
