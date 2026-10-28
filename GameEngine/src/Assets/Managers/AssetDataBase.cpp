/**
 * @file    AssetDataBase.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 */

#include "pch.h"
#include "Assets/Managers/AssetDataBase.h"

#include "Common/Random.h"

namespace RHEngine
{
	void AssetDataBase::ScanFile(const std::filesystem::path& root)
	{
		// rootディレクトリに含まれる全ファイル・ディレクトリを再帰的に調べる
		for (auto& file : std::filesystem::recursive_directory_iterator(root))
		{
			if (file.path().extension() == ".aux") continue;

			if (file.is_directory()) continue;

			ScanOnceFile(file);
		}
	}

	void AssetDataBase::ScanOnceFile(const std::filesystem::path& path)
	{
		AssetAux aux{};

		auto auxPath = path.wstring() + L".aux";

		// 同名の.auxファイルがなければ
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

			// サブアセットをデータベースに登録する
			for (auto& subAsset : aux.subAssets)
			{
				// 仮想パスを作成
				// サブアセットの仮想パスはメインアセットのパスの末尾に、# + アセット名 を追加したものとします。
				// 例 : Model.obj#Mesh2
				std::filesystem::path vPath = path.string() + "#" + subAsset.assetType + "#" + subAsset.name;

				Register(subAsset.uuid, vPath);

				// SubIDとMainIDの関係を保存
				m_subIdToMainId[subAsset.uuid] = aux.uuid;
			}
		}

		Register(aux.uuid, path);

		m_uuidToAux[aux.uuid] = aux;
	}

	void AssetDataBase::SaveAux(AssetAux& asset, const std::filesystem::path& path)
	{
		m_auxFileRepository.SaveAux(asset, path.wstring() + L".aux");

		// 変換表のAuxも更新
		auto uuid = GetUUID(path);

		m_uuidToAux[uuid] = asset;

		// 再スキャン
		ScanOnceFile(path);
	}

	UUID AssetDataBase::GetMainUUID(UUID id)
	{
		// SubAssetならMainIDを取得し返す
		if (m_subIdToMainId.find(id) != m_subIdToMainId.end())
		{
			return m_subIdToMainId[id];
		}

		// MainAssetならそのまま返す
		return id;
	}

	UUID AssetDataBase::GetUUID(const std::filesystem::path& path) const
	{
		auto it = m_pathToUuid.find(path);

		return it == m_pathToUuid.end() ? UUID_NONE : m_pathToUuid.at(path);
	}

	const std::filesystem::path& AssetDataBase::GetPath(UUID uuid) const
	{
		static std::filesystem::path errorPath{};

		auto it = m_uuidToPath.find(uuid);

		return it == m_uuidToPath.end() ? errorPath : m_uuidToPath.at(uuid);
	}

	AssetAux AssetDataBase::GetAux(UUID uuid) const
	{
		auto it = m_uuidToAux.find(uuid);

		return it == m_uuidToAux.end() ? AssetAux{} : m_uuidToAux.at(uuid);
	}

	void AssetDataBase::ReName(const std::filesystem::path& old, const std::filesystem::path& next)
	{
		// 名前変更処理
		namespace fs = std::filesystem;
		
		// 本体ファイルとauxファイルの両方を変更することでUUIDが変更されないようにします
		fs::rename(old, next);
		fs::rename(fs::path(old.string() + ".aux"), fs::path(next.string() + ".aux"));

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
		// 0~10000はEngineのデフォルトで使用するAssetのIDとする
		UUID uuid = Random::Get<UUID>(10000, std::numeric_limits<UUID>::max());

		// リストにないUUIDが生成されるまでループ
		while(m_uuidToPath.contains(uuid))
		{
			// ランダム生成
			uuid = Random::Get<UUID>(10000, std::numeric_limits<UUID>::max());
		}

		// UUIDを返す
		return uuid;
	}

	void AssetDataBase::Register(UUID uuid, const std::filesystem::path& path)
	{
		// uuid->pathの変換表に追加
		m_uuidToPath[uuid] = path;

		// path->uuidの変換表に追加
		m_pathToUuid[path] = uuid;
	}
}	// namespace RHEngine
