/**
 * @file    AssetSaver.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/02
 */

#include "pch.h"
#include "AssetSaver.h"

#include <fstream>

#include "Assets/Types/Prefab.h"
#include "Assets/Types/PrefabImpl.h"

#include "Assets/Managers/AssetContext.h"
#include "Assets/Managers/AssetManager.h"
#include "Editor/Saver/ObjectSaver.h"

namespace RHEngine
{
	namespace Saver
	{
		void PrefabSaver(AssetBase* pAsset, const std::filesystem::path& path, AssetContext& ctx)
		{
			Prefab* pPrefab = static_cast<Prefab*>(pAsset);

			// jsonを取得
			nlohmann::json json = pPrefab->GetImpl()->GetJson();

			// 保存
			std::ofstream ofs(path);

			// 開けていたら
			if (ofs.is_open())
			{
				// パス
				ofs << json.dump(4);
			}

			// 閉じる
			ofs.close();
		}

		void AssetSaverAsProperty(AssetBase* pAsset, const std::filesystem::path& path, const AssetManager& assetManager)
		{
			// セーバーを生成
			ObjectSaver saver(assetManager);

			// ファイルにセーブ
			saver.SavePropertyToFile(path, pAsset);
		}
	}
}	// namespace RHEngine
