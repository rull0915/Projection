/**
 * @file    ObjectSaver.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/29
 */

#include "pch.h"

#include <filesystem>
#include <fstream>

#include "ThirdParty/nameof/nameof.hpp"
#include "Editor/Saver/ObjectSaver.h"
#include "GameObject/GameObject.h"
#include "Managers/ObjectManager.h"
#include "Scene/Scene.h"
#include "Common/Property/EnumRegistry.h"
#include "Common/Property/AssetPropertyRegistry.h"
#include "Common/ObjectReference.h"

namespace RHEngine
{
	using namespace nlohmann;

	nlohmann::json ObjectSaver::SaveProperty(Property& property)
	{
		json js;

		// 型によって分岐
		switch (property.type)
		{
			// int
		case PropertyType::Int:
			js = *(static_cast<int*>(property.value));
			break;

			// float
		case PropertyType::Float:
			js = *(static_cast<float*>(property.value));
			break;

			// bool
		case PropertyType::Bool:
			js = *(static_cast<bool*>(property.value));
			break;

			// string
		case PropertyType::String:
			js = *(static_cast<std::string*>(property.value));
			break;

			// Vector2
		case PropertyType::Vector2: {
			auto v = (static_cast<DirectX::SimpleMath::Vector2*>(property.value));
			js = { v->x, v->y };
			break;
		}
			// Vector3
		case PropertyType::Vector3: {
			auto v = (static_cast<DirectX::SimpleMath::Vector3*>(property.value));
			js = { v->x, v->y, v->z };
			break;
		}

			// Vector4
		case PropertyType::Vector4: {
			auto v = (static_cast<DirectX::SimpleMath::Vector4*>(property.value));
			js = { v->x, v->y, v->z, v->w };
			break;
		}

			// Quaternion
		case PropertyType::Quaternion: {
			auto v = (static_cast<DirectX::SimpleMath::Quaternion*>(property.value));
			js = { v->x, v->y, v->z, v->w };
			break;
		}
			// Color
		case PropertyType::Color: {
			auto v = (static_cast<DirectX::SimpleMath::Color*>(property.value));
			js = { v->x, v->y, v->z, v->w };
			break;
		}
			// PropertyObject
		case PropertyType::Object: {
			auto v = (static_cast<PropertyObject*>(property.value));
			js = SavePropertyObject(*v);
			break;
		}
			// Enum
		case PropertyType::Enum: {
			auto& registry = EnumRegistry::Instance();
			js = registry.GetCurrentName(property.typeIndex, property.value);
			break;
		}
			// AssetHandle
		case PropertyType::AssetHandle: {
			auto& registry = AssetPropertyRegistry::Instance();
			js = registry.GetUUID(property.typeIndex, property.value, m_assetResolver);	// UUIDを保存
			break;
		}
			// ObjRef
		case PropertyType::ObjectRef: {
			auto v = (static_cast<RefBase*>(property.value));
			js = v->GetUUID();
			break;
		}
			// Array
		case PropertyType::Array: {
			auto& registry = ArrayRegistry::Instance();
			size_t size = registry.GetSize(property.typeIndex, property.value);
			js["size"] = size;
			for (size_t i = 0; i < size; ++i)
			{
				Property p = registry.GetProperty(property.typeIndex, property.value, i);
				js["array"].push_back(SaveProperty(p));
			}
			break;
		}
			// Range
		case PropertyType::Range:
			js = static_cast<Range*>(property.value)->GetValue();
			break;
		default:
			break;
		}

		return js;
	}

	nlohmann::json ObjectSaver::SavePropertyObject(PropertyObject& obj)
	{
		json js;

		// 全プロパティを調べる
		for (auto& property : obj.GetProperties())
		{
			auto j = SaveProperty(property);

			if (!j.empty())
			{
				js[property.name] = j;
			}
		}

		return js;
	}

	json ObjectSaver::SaveObject(GameObject* pObject)
	{
		// GameObject部分を保存
		json j = SavePropertyObject(*pObject);

		// UUIDを保存
		j["UUID"] = pObject->GetUUID();

		// コンポーネントを全て調べる
		for (auto& pComponent : pObject->GetAllComponents())
		{
			// jsonを生成
			json compJson = SavePropertyObject(*pComponent);

			// コンポーネント名を取得
			auto componentName = NAMEOF_SHORT_TYPE_RTTI(*pComponent);

			j["Components"].push_back(
				{
					{ "Type", componentName.data() },
					{ "Data", compJson },
					{ "UUID", pComponent->GetUUID() }
				}
			);
		}

		j["Children"] = nlohmann::json::array();

		// Transformを取得
		if (auto* pTransform = pObject->GetComponent<Transform>())
		{
			// 再帰的に子供を保存
			for (auto& pChild : pTransform->GetChildren())
			{
				j["Children"].push_back(SaveObject(static_cast<GameObject*>(pChild->GetOwn())));
			}

			// WorldObject
			j["IsWorld"] = true;
		}
		// RectTransformの場合も同様
		if (auto* pRectTransform = pObject->GetComponent<RectTransform>())
		{
			// 再帰的に子供を保存
			for (auto& pChild : pRectTransform->GetChildren())
			{
				j["Children"].push_back(SaveObject(static_cast<GameObject*>(pChild->GetOwn())));
			}

			// UIObject
			j["IsWorld"] = false;
		}

		return j;
	}

	nlohmann::json ObjectSaver::SaveObjectManager(const ObjectManager* pObjManager)
	{
		json j;
		j["GameObjects"] = nlohmann::json::array();

		// 保存
		for (auto& object : pObjManager->GetAllObject())
		{
			// シリアライズ可能なら
			if (object->IsSerializable())
			{
				// 親がnullの場合
				if (!object->GetParent())
				{
					// jsonを追加
					j["GameObjects"].push_back(SaveObject(object.get()));
				}
			}
		}

		return j;
	}

	nlohmann::json ObjectSaver::SaveScene(const Scene* pScene)
	{
		json j;

		j["World"] = SaveObjectManager(pScene->GetObjectManager());

		return j;
	}

	void ObjectSaver::SaveObjectToFile(const std::filesystem::path& filePath, GameObject* pObject)
	{
		std::ofstream ofs(filePath);

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << SaveObject(pObject).dump(4);
		}

		// 閉じる
		ofs.close();
	}

	void ObjectSaver::SaveSceneToFile(const std::filesystem::path& filePath, Scene* pScene)
	{
		std::ofstream ofs(filePath);

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << SaveScene(pScene).dump(4);
		}

		// 閉じる
		ofs.close();
	}

	void ObjectSaver::SavePropertyToFile(const std::filesystem::path& filePath, PropertyObject* pProperty)
	{
		std::ofstream ofs(filePath);

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << SavePropertyObject(*pProperty).dump(4);
		}

		// 閉じる
		ofs.close();
	}
}	// namespace RHEngine
