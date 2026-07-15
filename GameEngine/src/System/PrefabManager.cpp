//====================================================//
// ファイル名  : PrefabManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/PrefabManager.h"

#include "Editor/Loader/ObjectLoader.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void PrefabManager::AddPrefab(const std::string& key, const std::wstring& path)
	{
		// 存在しないキーなら追加する
		if (m_prefabList.find(key) == m_prefabList.end())
		{
			m_prefabList.insert({ key, path });
		}
	}

	const std::wstring PrefabManager::GetPrefab(const std::string& key)
	{
		// 存在するキーなら返す
		if (m_prefabList.find(key) != m_prefabList.end())
		{
			return m_prefabList[key];
		}

		return std::wstring();
	}

	void PrefabManager::LoadPrefab(const std::string& key, GameObject* obj)
	{
		// 取得
		auto& path = GetPrefab(key);

		// 取得できなければ読み込まない
		if (path.empty()) return;

		// 読み込み
		ObjectLoader::LoadFromFile(path, obj);
	}
}	// namespace REngine
