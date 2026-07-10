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

#include "Managers/UI/Canvas.h"
#include "Managers/UI/UIManager.h"
#include "Managers/ObjectManager.h"

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

		default:
			break;
		}
	}
}

void ObjectLoader::LoadObject(const nlohmann::json& json, GameObject* obj, Scene* pScene)
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

	// 子供をロード
	for (auto& child : json["Children"])
	{
		// 生成
		GameObject* chObj = pScene->Generate();

		// 親を自分に
		chObj->GetComponent<Transform>()->SetParent(obj->GetComponent<Transform>());

		// 読み込み
		LoadObject(child, chObj, pScene);
	}
}

void ObjectLoader::LoadUIObject(const nlohmann::json& json, GameObject* obj, Canvas* canvas)
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

	// 子供をロード
	for (auto& child : json["Children"])
	{
		// 生成
		GameObject* chObj = canvas->Generate();

		// 親を自分に
		chObj->GetComponent<RectTransform>()->SetParent(obj->GetComponent<RectTransform>());

		// 読み込み
		LoadUIObject(child, chObj, canvas);
	}
}

void ObjectLoader::LoadCanvas(const nlohmann::json& json, Canvas* canvas)
{
	// プロパティ部分をロード
	LoadProperty(json, *canvas);

	// ゲームオブジェクトをロード
	for (auto& object : json["GameObjects"])
	{
		// 読み込み
		LoadUIObject(object, canvas->Generate(), canvas);
	}
}

void ObjectLoader::LoadUIManager(const nlohmann::json& json, UIManager* manager)
{
	// キャンバスをループ
	for (auto& canvas : json["Canvases"])
	{
		// 生成
		Canvas* pCanvas = manager->CreateCanvas();

		// ロード
		LoadCanvas(canvas, pCanvas);
	}
}

void ObjectLoader::LoadObjectManager(const nlohmann::json& json, Scene* pScene)
{
	// オブジェクトをループ
	for (auto& obj : json["GameObjects"])
	{
		// 生成
		GameObject* object = pScene->Generate();

		// ロード
		LoadObject(obj, object, pScene);
	}
}

void ObjectLoader::LoadScene(const nlohmann::json& json, Scene* pScene)
{
	// Worldのロード
	LoadObjectManager(json["World"], pScene);

	// UIのロード
	LoadUIManager(json["UI"], pScene->GetUIManager());
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
