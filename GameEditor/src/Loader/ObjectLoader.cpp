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

#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"

#include "GameObject/GameObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

using namespace nlohmann;

void ObjectLoader::Register(unsigned int id, Loader loader)
{
	// イテレータを取得
	auto it = m_funcMap.find(id);

	// 既に存在するキーなら
	if (it != m_funcMap.end())
	{
		// 何もしない
		return;
	}

	// 新規のキーなら追加
	m_funcMap.insert({ id, loader });
}

void ObjectLoader::Load(const nlohmann::json& json, ComponentBase* component)
{
	unsigned int id = component->GetID();

	// イテレータを取得
	auto it = m_funcMap.find(id);

	// 存在しないキーなら
	if (it == m_funcMap.end())
	{
		// 何もしない
		return;
	}

	// 既存のキーなら実行
	return it->second(json, component);
}

void ObjectLoader::LoadObject(const nlohmann::json & js, GameObject * obj)
{
	// GameObjectの設定をロード
	obj->SetName(js["Name"]);
	obj->SetTag(js["Tag"]);
	obj->SetActive(js["IsActive"]);

	// 保存されているコンポーネントをループ
	for (const auto& component : js["Components"])
    {
		// コンポーネント名を取得
        std::string type = component["Type"];

		// コンポーネントデータを取得
        const json& data = component["Data"];

		// 生成
        ComponentBase* comp = ComponentFactory::Create(type, obj);

		// 生成したコンポーネントをロード
        if (comp)
        {
            Load(data, comp);
        }
    }
}

void ObjectLoader::LoadFromFile(const std::wstring& filePath, GameObject* obj)
{
	std::ifstream ifs(std::filesystem::path(filePath).c_str());

	// 開けていたら
	if (ifs.is_open())
	{
		json j;

		// パス
		ifs >> j;

		// ロード
		LoadObject(j, obj);
	}

	// 閉じる
	ifs.close();
}
