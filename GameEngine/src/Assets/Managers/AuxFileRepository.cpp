//====================================================//
// ファイル名  : AuxFileRepository.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要       : AssetAuxと.auxファイルの読み書きを行うクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Managers/AuxFileRepository.h"

#include <fstream>
#include "nlohmann/json.hpp"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void AuxFileRepository::SaveAux(AssetAux& aux, const std::wstring& path)
	{
		// jsonを用意
		nlohmann::json j;

		// 書き込む
		j["UUID"] = aux.uuid;
		j["AssetType"] = aux.assetType;

		// ファイルを書き込み専用で開く
		std::ofstream ofs(path);

		// 開けていたら
		if (ofs.is_open())
		{
			// パス
			ofs << j.dump(4);
		}

		// 閉じる
		ofs.close();
	}

	void AuxFileRepository::LoadAux(AssetAux& aux, const std::wstring& path)
	{
		// ファイルを読み取り専用で開く
		std::ifstream ifs(path);

		// 開けていたら
		if (ifs.is_open())
		{
			// jsonへ変換
			nlohmann::json j;
			ifs >> j;

			// ロード
			aux.uuid = j["UUID"];
			aux.assetType = j["AssetType"];
		}

		// 閉じる
		ifs.close();
	}
}	// namespace REngine
