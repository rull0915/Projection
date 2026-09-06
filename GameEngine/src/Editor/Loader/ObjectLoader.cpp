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
#include "Common/ObjectReference.h"
#include "Common/UUID.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void ObjectLoader::LoadProperty(const nlohmann::json& json, Property& property, Scene* pScene)
	{
		// 型によって分岐
		switch (property.type)
		{
			// int
		case PropertyType::Int:
			*(static_cast<int*>(property.value)) = json;
			break;

			// float
		case PropertyType::Float:
			*(static_cast<float*>(property.value)) = json;
			break;

			// bool
		case PropertyType::Bool:
			*(static_cast<bool*>(property.value)) = json;
			break;

			// string
		case PropertyType::String:
			*(static_cast<std::string*>(property.value)) = json;
			break;

			// Vector2
		case PropertyType::Vector2:
			*(static_cast<DirectX::SimpleMath::Vector2*>(property.value)) = { json[0], json[1] };
			break;

			// Vector3
		case PropertyType::Vector3:
			*(static_cast<DirectX::SimpleMath::Vector3*>(property.value)) = { json[0], json[1], json[2] };
			break;

			// Quaternion
		case PropertyType::Quaternion:
			*(static_cast<DirectX::SimpleMath::Quaternion*>(property.value)) = { json[0], json[1], json[2], json[3] };
			break;

			// Color
		case PropertyType::Color:
			*(static_cast<DirectX::SimpleMath::Color*>(property.value)) = { json[0], json[1], json[2], json[3] };
			break;

			// PropertyObject
		case PropertyType::Object:
			LoadPropertyObject(json, *(static_cast<PropertyObject*>(property.value)), pScene);
			break;

			// Enum
		case PropertyType::Enum: {
			auto& registry = EnumRegistry::Instance();
			registry.SetByName(property.typeIndex, property.value, json);
			break;
		}
			// AssetHandle
		case PropertyType::AssetHandle: {
			auto& registry = AssetPropertyRegistry::Instance();
			UnTypeHandle handle = m_assetManager.LoadFromUUID(json);	// UUIDからHandleを取得
			registry.Assign(property.typeIndex, property.value, handle);	// 変更
			break;
		}
			// ObjRef
		case PropertyType::ObjectRef: {
			(static_cast<RefBase*>(property.value))->SetUUID(json);
			if (pScene) pScene->RegisterLateResolve(static_cast<RefBase*>(property.value));	// シーンに参照の遅延解決をリクエスト
			break;
		}
			// Array
		case PropertyType::Array: {
			// jsonを取得
			if (!json.contains("size")) return;	

			auto& registry = ArrayRegistry::Instance();	// レジストリを取得
			size_t size = json["size"];					// 配列のサイズを取得
			registry.Resize(property.typeIndex, property.value, size);	// リサイズ

			// 要素分ループ
			for (size_t i = 0; i < size; ++i)
			{
				// プロパティとしてロード
				Property obj = registry.GetProperty(property.typeIndex, property.value, i);
				LoadProperty(json["array"][i], obj, pScene);
			}
			break;
		}
		default:
			break;
		}
	}

	void ObjectLoader::LoadPropertyObject(const nlohmann::json& json, PropertyObject& obj, Scene* pScene)
	{
		// 登録されているプロパティを全て調べる
		for (auto& property : obj.GetProperties())
		{
			// 存在チェック
			if (!json.contains(property.name)) continue;

			// ロード
			LoadProperty(json[property.name], property, pScene);
		}
	}

	void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* obj, Scene* pScene)
	{
		// ゲームオブジェクト部分をロード
		LoadPropertyObject(json, *obj, pScene);

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
				LoadPropertyObject(js["Data"], *component, pScene);

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
			LoadPropertyObject(j, *obj, nullptr);
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
