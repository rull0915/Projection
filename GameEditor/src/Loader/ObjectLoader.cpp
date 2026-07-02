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

void ObjectLoader::LoadPropaty(const nlohmann::json& json, PropatyObject& obj)
{
	// 登録されているプロパティを全て調べる
	for (auto& propaty : obj.GetPropaties())
	{
		// 型によって分岐
		switch (propaty.type)
		{
			// int
		case PropatyType::Int:
			*(static_cast<int*>(propaty.value)) = json[propaty.name];
			break;

			// float
		case PropatyType::Float:
			*(static_cast<float*>(propaty.value)) = json[propaty.name];
			break;

			// bool
		case PropatyType::Bool:
			*(static_cast<bool*>(propaty.value)) = json[propaty.name];
			break;

			// string
		case PropatyType::String:
			*(static_cast<std::string*>(propaty.value)) = json[propaty.name];
			break;

			// Vector2
		case PropatyType::Vector2:
			*(static_cast<DirectX::SimpleMath::Vector2*>(propaty.value)) = { json[propaty.name][0], json[propaty.name][1] };
			break;

			// Vector3
		case PropatyType::Vector3:
			*(static_cast<DirectX::SimpleMath::Vector3*>(propaty.value)) = { json[propaty.name][0], json[propaty.name][1], json[propaty.name][2] };
			break;

			// Quaternion
		case PropatyType::Quaternion:
			*(static_cast<DirectX::SimpleMath::Quaternion*>(propaty.value)) = { json[propaty.name][0], json[propaty.name][1], json[propaty.name][2], json[propaty.name][3] };
			break;

		default:
			break;
		}
	}
}

void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* obj)
{
	// ゲームオブジェクト部分をロード
	LoadPropaty(json, *obj);

	// コンポーネントをロード
	for (auto& js : json["Components"])
	{
		// 生成
		ComponentBase* component = ComponentFactory::Create(js["Type"], obj);

		// ロード
		LoadPropaty(js["Data"], *component);
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
