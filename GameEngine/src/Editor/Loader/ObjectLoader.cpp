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

#include "Editor/Loader/ObjectLoader.h"
#include "Editor/Loader/ComponentFactory.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include <filesystem>
#include <string>
#include "Common/Property/AssetPropertyRegistry.h"
#include "Common/Property/EnumRegistry.h"
#include "Common/UUID.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void ObjectLoader::LoadProperty(const nlohmann::json& json, PropertyObject& obj)
	{
		// 登録されているプロパティを全て調べる
		for (auto& property : obj.GetPropaties())
		{
			// 存在チェック
			if (!json.contains(property.name)) continue;

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

				// Color
			case PropertyType::Color:
				*(static_cast<DirectX::SimpleMath::Color*>(property.value)) = { json[property.name][0], json[property.name][1], json[property.name][2], json[property.name][3] };
				break;

				// PropertyObject
			case PropertyType::Object:
				LoadProperty(json[property.name], *(static_cast<PropertyObject*>(property.value)));
				break;

				// Enum
			case PropertyType::Enum: {
				auto& registry = EnumRegistry::Instance();
				registry.SetByName(property.typeIndex, property.value, json[property.name]);
				break;
			}
				// AssetHandle
			case PropertyType::AssetHandle: {
				auto& registry = AssetPropertyRegistry::Instance();
				UnTypeHandle handle = m_assetManager.LoadFromUUID(json[property.name]);	// UUIDからHandleを取得
				registry.Assign(property.typeIndex, property.value, handle);	// 変更
				break;
			}
			default:
				break;
			}
		}
	}

	void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* obj, Scene* pScene)
	{
		// ゲームオブジェクト部分をロード
		LoadProperty(json, *obj);

		// UUIDをロード
		UUID uuid = 0;
		if (json.contains("UUID"))
		{
			uuid = json["UUID"];
			obj->SetUUID(uuid);
		}

		// コンポーネントをロード
		for (auto& js : json["Components"])
		{
			// 生成
			ComponentBase* component = ComponentFactory::Create(js["Type"], obj);

			// ロード
			if (component)
			{
				LoadProperty(js["Data"], *component);

				// 変更時処理の呼び出し
				component->OnValidate();

				// UUIDをロード
				if (js.contains("UUID"))
				{
					uuid = js["UUID"];
					component->SetUUID(uuid);
				}
			}
		}

		// 子供をロード
		for (auto& child : json["Children"])
		{
			// 生成
			GameObject* chObj = nullptr;

			// UIかWorldかを調べる
			if (child.contains("IsWorld") && !child["IsWorld"])
			{
				chObj = pScene->GetFactory()->GenerateUI();

				// 親を自分に
				chObj->GetComponent<RectTransform>()->SetParent(obj->GetComponent<RectTransform>());
			}
			else
			{
				chObj = pScene->GetFactory()->Generate();

				// 親を自分に
				chObj->GetComponent<Transform>()->SetParent(obj->GetComponent<Transform>());
			}

			// 読み込み
			LoadObject(child, chObj, pScene);
		}
	}

	void ObjectLoader::LoadObjectManager(const nlohmann::json& json, Scene* pScene)
	{
		// オブジェクトをループ
		for (auto& obj : json["GameObjects"])
		{
			// 生成
			GameObject* object = nullptr;

			// UIかWorldかを調べる
			if (obj.contains("IsWorld") && !obj["IsWorld"])
			{
				object = pScene->GetFactory()->GenerateUI();
			}
			else
			{
				object = pScene->GetFactory()->Generate();
			}

			// ロード
			LoadObject(obj, object, pScene);
		}
	}

	void ObjectLoader::LoadScene(const nlohmann::json& json, Scene* pScene)
	{
		// Worldのロード
		LoadObjectManager(json["World"], pScene);
	}

	void ObjectLoader::LoadPropertyFromFile(const std::wstring& filePath, PropertyObject* obj)
	{
		std::ifstream ifs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ifs.is_open())
		{
			// jsonから読み取り
			nlohmann::json j;
			ifs >> j;

			// ロード
			LoadProperty(j, *obj);
		}

		// 閉じる
		ifs.close();
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
			LoadObject(j, obj, obj->GetScene());
		}

		// 閉じる
		ifs.close();
	}

	void ObjectLoader::LoadSceneFromFile(const std::wstring& filePath, Scene* scene)
	{
		std::ifstream ifs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ifs.is_open())
		{
			// jsonから読み取り
			nlohmann::json j;
			ifs >> j;

			// ロード
			LoadScene(j, scene);
		}

		// 閉じる
		ifs.close();
	}
}	// namespace REngine
