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

#include "GameObject/GameObject.h"
#include "Editor/Common/ClassNameGetter.h"

#include "Managers/ObjectManager.h"
#include "Managers/UI/UIManager.h"

#include "Scene/Scene.h"


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

	j["Children"] = nlohmann::json::array();

	// Transformを取得
	if (auto* tr = obj->GetComponent<Transform>())
	{
		// 再帰的に子供を保存
		for (auto& child : tr->GetChildren())
		{
			j["Children"].push_back(SaveObject(static_cast<GameObject*>(child->GetOwn())));
		}
	}
	// RectTransformの場合も同様
	if (auto* tr = obj->GetComponent<RectTransform>())
	{
		// 再帰的に子供を保存
		for (auto& child : tr->GetChildren())
		{
			j["Children"].push_back(SaveObject(static_cast<GameObject*>(child->GetOwn())));
		}
	}

	return j;
}

nlohmann::json ObjectSaver::SaveCanvas(const Canvas* canvas)
{
	// Property部分を保存
	json j = SaveProperty(*canvas);

	j["GameObjects"] = nlohmann::json::array();

	// ゲームオブジェクトを全て調べる
	for (auto& object : canvas->GetAllObjects())
	{
		if (!object->IsInvincible())
		{
			// 親がルートの場合
			if (object->GetComponent<RectTransform>()->GetParent() ==
				canvas->GetRootObject()->GetComponent<RectTransform>())
			{
				// 追加
				j["GameObjects"].push_back(SaveObject(object.get()));
			}
		}
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
			if (object->GetComponent<Transform>()->GetParent() == nullptr)
			{
				// jsonを追加
				j["GameObjects"].push_back(SaveObject(object.get()));
			}
		}
	}

	return j;
}

nlohmann::json ObjectSaver::SaveUIManager(const UIManager* uiManager)
{
	json j;
	j["Canvases"] = nlohmann::json::array();

	// 全キャンバスを取得
	for (auto& canvas : uiManager->GetAllCanvas())
	{
		// 追加
		j["Canvases"].push_back(SaveCanvas(canvas.get()));
	}

	return j;
}

nlohmann::json ObjectSaver::SaveScene(const Scene* scene)
{
	json j;
	
	j["World"] = SaveObjectManager(scene->GetObjectManager());
	j["UI"] = SaveUIManager(scene->GetUIManager());

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
