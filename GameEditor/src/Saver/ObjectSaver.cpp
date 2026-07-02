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

nlohmann::json ObjectSaver::SavePropaty(const PropatyObject& obj)
{
	json js;

	// 全プロパティを調べる
	for (auto& propaty : obj.GetPropaties())
	{
		// 型によって分岐
		switch (propaty.type)
		{
			// 型によって分岐
			switch (propaty.type)
			{
				// int
			case PropatyType::Int:
				js[propaty.name] = *(static_cast<int*>(propaty.value));
				break;

				// float
			case PropatyType::Float:
				js[propaty.name] = *(static_cast<float*>(propaty.value));
				break;

				// bool
			case PropatyType::Bool:
				js[propaty.name] = *(static_cast<bool*>(propaty.value));
				break;

				// string
			case PropatyType::String:
				js[propaty.name] = *(static_cast<std::string*>(propaty.value));
				break;

				// Vector2
			case PropatyType::Vector2: {
				auto v = (static_cast<DirectX::SimpleMath::Vector2*>(propaty.value));
				js[propaty.name] = { v->x, v->y };
				break;
			}
				// Vector3
			case PropatyType::Vector3: {
				auto v = (static_cast<DirectX::SimpleMath::Vector3*>(propaty.value));
				js[propaty.name] = { v->x, v->y, v->z };
				break;
			}
				// Quaternion
			case PropatyType::Quaternion: {
				auto v = (static_cast<DirectX::SimpleMath::Quaternion*>(propaty.value));
				js[propaty.name] = { v->x, v->y, v->z, v->w };
				break;
			}

			default:
				break;
			}
		}
	}

	return js;
}

json ObjectSaver::SaveObject(const GameObject* obj)
{
	json j;

	// GameObject部分を保存
	SavePropaty(*obj);

	// コンポーネントを全て調べる
	for (auto& component : obj->GetAllComponents())
	{
		// jsonを生成
		json compJson = SavePropaty(*component);

		// コンポーネント名を取得
		std::string componentName = ClassNameGetter::Get(*component);

		j["Components"].push_back(
			{
				{ "Type", componentName},
				{ "Data", compJson },
			}
		);
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
