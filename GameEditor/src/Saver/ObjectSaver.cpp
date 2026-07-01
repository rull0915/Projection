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
#include "Saver/ObjectSaver.h"

#include "GameObject/GameObject.h"
#include "Common/ClassNameGetter.h"

#include <fstream>
#include <filesystem>

//====================================================//
// 関数の実体宣言
//====================================================//

using namespace nlohmann;

void ObjectSaver::Register(unsigned int id, Saver saver)
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
	m_funcMap.insert({ id, saver });
}

json ObjectSaver::Save(ComponentBase* component)
{
	unsigned int id = component->GetID();

	// イテレータを取得
	auto it = m_funcMap.find(id);

	// 存在しないキーなら
	if (it == m_funcMap.end())
	{
		// 何もしない
		return json{};
	}

	// 既存のキーなら実行
	return it->second(component);
}

json ObjectSaver::SaveObject(GameObject* obj)
{
	json j;

	j["Name"] = obj->GetName();
	j["Tag"] = obj->GetTag();
	j["IsActive"] = obj->IsActive();

	// コンポーネントをすべて取得
	auto& components = obj->GetAllComponents();

	// 1つのコンポーネントを保存するラムダ式
	auto saveComponent = [&](ComponentBase* component)
		{
			// jsonを生成
			json compJson = Save(component);
			if (compJson.is_null()) compJson = {};

			// コンポーネント名を取得
			std::string componentName = ClassNameGetter::Get(*component);

			j["Components"].push_back(
				{
					{ "Type", componentName},
					{ "Data", compJson },
				}
				);
		};

	// コンポーネントを全部調べる
	for (auto& component : components)
	{
		saveComponent(component);
	}

	return j;
}

void ObjectSaver::SaveToFile(const std::wstring& filePath, GameObject* obj)
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
