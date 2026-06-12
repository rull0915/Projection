//====================================================//
// ファイル名  : GameObjectSaver.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/11
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameObjectSaver.h"
#include "../Components/BaseComponent.h"
#include "../Components/ComponentID.h"

#include "ComponentFactory.h"

#include <memory>
#include <vector>
#include <fstream>

//====================================================//
// 関数の実体宣言
//====================================================//

void GameObjectSaver::SaveObject(GameObject* obj, const std::string& fileName)
{
	// ファイルを書き込み専用で開く
	std::ofstream file(fileName);

	// ファイルが開けていたら
	if (file.is_open())
	{
		// 全コンポーネントを取得
		std::vector<std::unique_ptr<BaseComponent>>& components = obj->GetComponentContainer().GetAll();

		// 書き込むjsonを用意
		json js;

		// トランスフォームのデータを書き込むjsonを用意
		json transformJson;

		// 保存
		obj->GetComponent<Transform>()->Save(transformJson);

		// リストに追加
		js["Components"].push_back(
			{
				{ "ComponentID", static_cast<int>(ComponentID::Transform) },
				{ "Data", transformJson }
			}
		);

		// 全てのコンポーネントを書き込む
		for (auto& component : components)
		{
			// 各コンポーネントのデータを書き込むjsonを用意
			json componentJson;

			// 保存
			component->Save(componentJson);

			// リストに追加
			js["Components"].push_back(
				{
					{ "ComponentID", static_cast<int>(component->GetID()) },
					{ "Data", componentJson }
				}
			);
		}

		// タグ情報
		js["Tag"] = obj->GetTag();

		// 書き込む
		file << js.dump(4);

		// ファイルを閉じる
		file.close();
	}
}

void GameObjectSaver::LoadObject(GameObject* obj, const std::string& fileName)
{
	// ファイルを読み取り専用で開く
	std::ifstream file(fileName);

	// jsonオブジェクトへ出力
	json js;
	if(file.is_open()) file >> js;
	file.close();

	// コンポーネントをリセット
	obj->GetComponentContainer().AllRemove();

	// jsonから読み取ってコンポーネントを追加していく
	for (auto& component : js["Components"])
	{
		// コンポーネントを追加
		BaseComponent* comp = ComponentFactory::CreateComponent(obj, static_cast<ComponentID>(component["ComponentID"]));

		// jsonから読み取ったデータで値をロードする
		comp->Load(component["Data"]);
	}

	// タグをセット
	obj->SetTag(js["Tag"]);
}
