/**
 * @file    ObjectLoader.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/30
 */

#include "pch.h"

#include <fstream>
#include <filesystem>
#include <string>

#include "Editor/Loader/ObjectLoader.h"
#include "Editor/Loader/ComponentFactory.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/ObjectFactory.h"
#include "Common/Property/AssetPropertyRegistry.h"
#include "Common/Property/EnumRegistry.h"
#include "Common/ObjectReference.h"
#include "Common/UUID.h"
#include "Math/Range.h"

namespace RHEngine
{
	void ObjectLoader::LoadProperty(const nlohmann::json& json, Property& property, Scene* pScene)
	{
		// 型によって分岐
		switch (property.type)
		{
			// int
		case PropertyType::Int:
			*(static_cast<int*>(property.value)) = json.get<int>();
			break;

			// float
		case PropertyType::Float:
			*(static_cast<float*>(property.value)) = json.get<float>();
			break;

			// bool
		case PropertyType::Bool:
			*(static_cast<bool*>(property.value)) = json.get<bool>();
			break;

			// string
		case PropertyType::String:
			*(static_cast<std::string*>(property.value)) = json.get<std::string>();
			break;

			// Vector2
		case PropertyType::Vector2:
			*(static_cast<DirectX::SimpleMath::Vector2*>(property.value)) = { json[0], json[1] };
			break;

			// Vector3
		case PropertyType::Vector3:
			*(static_cast<DirectX::SimpleMath::Vector3*>(property.value)) = { json[0], json[1], json[2] };
			break;

			// Vector4
		case PropertyType::Vector4:
			*(static_cast<DirectX::SimpleMath::Vector4*>(property.value)) = { json[0], json[1], json[2], json[3] };
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
			// Range
		case PropertyType::Range:
			*(static_cast<Range*>(property.value)) = json.get<float>();
			break;

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

	void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* pObject, Scene* pScene)
	{
		// ゲームオブジェクト部分をロード
		LoadPropertyObject(json, *pObject, pScene);

		// UUIDをロード
		UUID uuid = 0;
		if (json.contains("UUID"))
		{
			uuid = json["UUID"];
			pObject->SetUUID(uuid);
		}

		// コンポーネントをロード
		for (auto& js : json["Components"])
		{
			// 生成
			ComponentBase* pComponent = ComponentFactory::Create(js["Type"], pObject);

			// ロード
			if (pComponent)
			{
				LoadPropertyObject(js["Data"], *pComponent, pScene);

				// 変更時処理の呼び出し
				pComponent->OnValidate();

				// UUIDをロード
				if (js.contains("UUID"))
				{
					uuid = js["UUID"];
					pComponent->SetUUID(uuid);
				}
			}
		}

		// 子供をロード
		for (auto& child : json["Children"])
		{
			// 生成
			GameObject* childObject = nullptr;

			// UIかWorldかを調べる
			if (child.contains("IsWorld") && !child["IsWorld"])
			{
				childObject = pScene->GetFactory()->GenerateUI();

				// 親を自分に
				childObject->GetComponent<RectTransform>()->SetParent(pObject->GetComponent<RectTransform>());
			}
			else
			{
				childObject = pScene->GetFactory()->Generate();

				// 親を自分に
				childObject->GetComponent<Transform>()->SetParent(pObject->GetComponent<Transform>());
			}

			// 読み込み
			LoadObject(child, childObject, pScene);
		}
	}

	void ObjectLoader::LoadObjectManager(const nlohmann::json& json, Scene* pScene)
	{
		// オブジェクトをループ
		for (auto& obj : json["GameObjects"])
		{
			// 生成
			GameObject* pObject = nullptr;

			// UIかWorldかを調べる
			if (obj.contains("IsWorld") && !obj["IsWorld"])
			{
				pObject = pScene->GetFactory()->GenerateUI();
			}
			else
			{
				pObject = pScene->GetFactory()->Generate();
			}

			// ロード
			LoadObject(obj, pObject, pScene);
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

	void ObjectLoader::LoadObjectFromFile(const std::wstring& filePath, GameObject* pObject)
	{
		std::ifstream ifs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ifs.is_open())
		{
			// jsonから読み取り
			nlohmann::json j;
			ifs >> j;

			// ロード
			LoadObject(j, pObject, pObject->GetScene());
		}

		// 閉じる
		ifs.close();
	}

	void ObjectLoader::LoadSceneFromFile(const std::wstring& filePath, Scene* pScene)
	{
		std::ifstream ifs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ifs.is_open())
		{
			// jsonから読み取り
			nlohmann::json j;
			ifs >> j;

			// ロード
			LoadScene(j, pScene);
		}

		// 閉じる
		ifs.close();
	}
}	// namespace RHEngine
