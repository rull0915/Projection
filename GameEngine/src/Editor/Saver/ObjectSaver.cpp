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
#include <filesystem>
#include <fstream>

#include "Editor/Saver/ObjectSaver.h"
#include "ThirdParty/nameof/nameof.hpp"

#include "GameObject/GameObject.h"
#include "Managers/ObjectManager.h"
#include "Scene/Scene.h"

#include "Common/Property/EnumRegistry.h"
#include "Common/Property/AssetPropertyRegistry.h"
#include "Common/ObjectReference.h"

namespace REngine
{
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
				 // Color
			case PropertyType::Color: {
				auto v = (static_cast<DirectX::SimpleMath::Color*>(property.value));
				js[property.name] = { v->x, v->y, v->z, v->w };
				break;
			}
				// PropertyObject
			case PropertyType::Object: {
				auto v = (static_cast<PropertyObject*>(property.value));
				js[property.name] = SaveProperty(*v);
				break;
			}
				// Enum
			case PropertyType::Enum: {
				auto& registry = EnumRegistry::Instance();
				js[property.name] = registry.GetCurrentName(property.typeIndex, property.value);
				break;
			}
				// AssetHandle
			case PropertyType::AssetHandle: {
				auto& registry = AssetPropertyRegistry::Instance();
				js[property.name] = registry.GetUUID(property.typeIndex, property.value, m_assetManager);	// UUIDを保存
				break;
			}
				// ObjRef
			case PropertyType::ObjectRef: {
				auto v = (static_cast<RefBase*>(property.value));
				js[property.name] = v->GetUUID();
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
		// GameObject部分を保存
		json j = SaveProperty(*obj);

		// UUIDを保存
		j["UUID"] = obj->GetUUID();

		// コンポーネントを全て調べる
		for (auto& component : obj->GetAllComponents())
		{
			// jsonを生成
			json compJson = SaveProperty(*component);

			// コンポーネント名を取得
			auto componentName = NAMEOF_SHORT_TYPE_RTTI(*component);

			j["Components"].push_back(
				{
					{ "Type", componentName.data() },
					{ "Data", compJson },
					{ "UUID", component->GetUUID() }
				}
			);
		}

		j["Children"] = nlohmann::json::array();

		// Transformを取得
		if (auto* tr = obj->GetComponent<Transform>())
		{
			// 再帰的に子供を保存
			for (auto& child : tr->GetChildren())
			{
				j["Children"].push_back(SaveObject(static_cast<GameObject*>(child->GetOwn())));
			}

			// WorldObject
			j["IsWorld"] = true;
		}
		// RectTransformの場合も同様
		if (auto* tr = obj->GetComponent<RectTransform>())
		{
			// 再帰的に子供を保存
			for (auto& child : tr->GetChildren())
			{
				j["Children"].push_back(SaveObject(static_cast<GameObject*>(child->GetOwn())));
			}

			// UIObject
			j["IsWorld"] = false;
		}

		return j;
	}

	nlohmann::json ObjectSaver::SaveObjectManager(const ObjectManager* objManager)
	{
		json j;
		j["GameObjects"] = nlohmann::json::array();

		// 保存
		for (auto& object : objManager->GetAllObject())
		{
			if (!object->IsInvincible())
			{
				// 親がnullの場合
				if (auto* t = object->GetComponent<Transform>())
				{
					// jsonを追加
					if (!t->GetParent()) j["GameObjects"].push_back(SaveObject(object.get()));
				}
				else if (auto* t = object->GetComponent<RectTransform>())
				{
					// jsonを追加
					if (!t->GetParent()) j["GameObjects"].push_back(SaveObject(object.get()));
				}
			}
		}

		return j;
	}

	nlohmann::json ObjectSaver::SaveScene(const Scene* scene)
	{
		json j;

		j["World"] = SaveObjectManager(scene->GetObjectManager());

		return j;
	}

	void ObjectSaver::SaveObjectToFile(const std::wstring& filePath, GameObject* obj)
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

	void ObjectSaver::SaveSceneToFile(const std::wstring& filePath, Scene* scene)
	{
		std::ofstream ofs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << SaveScene(scene).dump(4);
		}

		// 閉じる
		ofs.close();
	}

	void ObjectSaver::SavePropertyToFile(const std::wstring& filePath, PropertyObject* property)
	{
		std::ofstream ofs(std::filesystem::path(filePath).c_str());

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << SaveProperty(*property).dump(4);
		}

		// 閉じる
		ofs.close();
	}
}	// namespace REngine
