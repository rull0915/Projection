//====================================================//
// ファイル名  : AssetDataBase.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要       : AssetとUUIDの双変換を行うクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Managers/AssetDataBase.h"

#include "Common/Random.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void AssetDataBase::ScanFile(const std::filesystem::path& root, const AssetTypeManager& typeManager)
	{
		// rootディレクトリに含まれる全ファイル・ディレクトリを再帰的に調べる
		for (auto& file : std::filesystem::recursive_directory_iterator(root))
		{
			// auxファイルは調べない
			if (file.path().extension() == ".aux") continue;

			// 同名のauxファイルが存在するかを調べる
			auto auxPath = file.path().wstring() + L".aux";

			// 存在しなければ
			if (!std::filesystem::exists(auxPath))
			{
				// 新しくAuxを生成する
				AssetAux aux{
					GenerateUUID(),
					typeManager.GetAssetType(file.path())
				};

				// .auxファイルへ書き込む
				m_auxFileRepository.SaveAux(aux, file.path());

				// 変換表へ登録
				Register(aux.uuid, file.path());
			}
			// 存在していれば
			else
			{
				// auxを用意
				AssetAux aux{};

				// .auxファイルから読み取る
				m_auxFileRepository.LoadAux(aux, file.path());

				// 変換表へ登録
				Register(aux.uuid, file.path());
			}
		}
	}

	UUID AssetDataBase::GenerateUUID()
	{
		// ランダムで生成する（衝突する確率は1 / 2^64のため考慮しない）
		return Random::Get<UUID>(0, static_cast<uint64_t>(-1));
	}
}	// namespace REngine
