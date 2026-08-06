//====================================================//
// ファイル名  : AssetSaver.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/02
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "AssetSaver.h"

#include <fstream>
#include "Assets/Types/PrefabImpl.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Saver
	{
		void PrefabSaver(AssetBase* base, const std::filesystem::path& path)
		{
			Prefab* prefab = static_cast<Prefab*>(base);

			// jsonを取得
			nlohmann::json json = prefab->GetImpl()->GetJson();

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

		void AssetSaverAsProperty(AssetBase* asset, const std::filesystem::path& path, AssetManager& assetManager)
		{
			// セーバーを生成
			ObjectSaver saver(assetManager);

			// ファイルにセーブ
			saver.SavePropertyToFile(path, asset);
		}
	}
}	// namespace REngine
