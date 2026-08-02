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

#include <fstream>
#include "Common/Random.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void AssetDataBase::ScanFile(const std::filesystem::path& root)
	{
		// rootディレクトリに含まれる全ファイル・ディレクトリを再帰的に調べる
		for (auto& file : std::filesystem::recursive_directory_iterator(root))
		{
			// auxファイルは調べない
			if (file.path().extension() == ".aux") continue;

			// フォルダは調べない
			if (file.is_directory()) continue;

			// auxの調査
			ScanOnceFile(file);
		}
	}

	void AssetDataBase::ScanOnceFile(const std::filesystem::path& path)
	{
		// 同名のauxファイルが存在するかを調べる
		auto auxPath = path.wstring() + L".aux";

		// 対応するaux
		AssetAux aux;

		// 存在しなければ
		if (!std::filesystem::exists(auxPath))
		{
			// UUIDを生成
			aux.uuid = GenerateUUID();

			// タイプを取得
			aux.assetType = m_assetTypeManager.GetAssetType(path);

			// .auxファイルへ書き込む
			m_auxFileRepository.SaveAux(aux, auxPath);
		}
		// 存在していれば
		else
		{
			// .auxファイルから読み取る
			m_auxFileRepository.LoadAux(aux, auxPath);
		}

		// 変換表へ登録
		Register(aux.uuid, path);
	}

	void AssetDataBase::ReName(const std::filesystem::path& old, const std::filesystem::path& next)
	{
		// 名前変更処理
		namespace fs = std::filesystem;
		
		// 本体ファイルとauxファイルの両方を変更することでUUIDが変更されないようにします
		std::filesystem::rename(old, next);
		std::filesystem::rename(fs::path(old.string() + ".aux"), fs::path(next.string() + ".aux"));

		// 以前のファイルのUUIDを取得
		UUID uuid = GetUUID(old);

		// 古い対応表を削除
		m_pathToUuid.erase(old);
		m_uuidToPath.erase(uuid);

		// 新しい対応表を作成
		Register(uuid, next);
	}

	void AssetDataBase::Move(const std::filesystem::path & old, const std::filesystem::path & next)
	{}

	void AssetDataBase::Delete(const std::filesystem::path & path)
	{
		std::filesystem::remove(path);
		std::filesystem::remove(std::filesystem::path(path.string() + ".aux"));
	}

	UUID AssetDataBase::GenerateUUID()
	{
		UUID uuid;

		// リストにないUUIDが生成されるまでループ
		while(m_uuidToPath.contains(uuid))
		{
			// ランダム生成
			uuid = Random::Get<UUID>(0, std::numeric_limits<UUID>::max());
		}

		// UUIDを返す
		return uuid;
	}
}	// namespace REngine
