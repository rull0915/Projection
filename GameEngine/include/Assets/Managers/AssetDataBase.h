//====================================================//
// ファイル名   : AssetDataBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : AssetとUUIDの双変換を行うクラス
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <filesystem>

#include "AuxFileRepository.h"
#include "AssetTypeManager.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetDataBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ファイルへの読み書きをするインスタンス
		AuxFileRepository m_auxFileRepository;

		// タイプ管理クラス
		const AssetTypeManager& m_assetTypeManager;

		// 相互変換のための対応表

		// uuidからファイルパス
		std::unordered_map<UUID, std::filesystem::path> m_uuidToPath;

		// ファイルパスからuuid
		std::unordered_map<std::filesystem::path, UUID> m_pathToUuid;

		// UUIDからAux
		std::unordered_map<UUID, AssetAux> m_uuidToAux;

		// サブアセットIDからメインアセットIDを取得するマップ
		std::unordered_map<UUID, UUID> m_subIdToMainId;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetDataBase(const AssetTypeManager& assetTypeManager)
			: m_assetTypeManager{ assetTypeManager }
		{
		}
		~AssetDataBase() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ファイルを走査しauxの生成を行う関数
		void ScanFile(const std::filesystem::path& root);

		// 特定のファイルに対してauxの生成を行う関数
		void ScanOnceFile(const std::filesystem::path& path);

		// auxファイルを保存する関数
		void SaveAux(AssetAux& asset, const std::filesystem::path& path)
		{
			m_auxFileRepository.SaveAux(asset, path.wstring() + L".aux");

			// 変換表のAuxも更新
			auto uuid = GetUUID(path);

			m_uuidToAux[uuid] = asset;
		}

		// MainAssetのUUIDを取得する関数
		UUID GetMainUUID(UUID id)
		{
			// SubAssetならMainIDを取得し返す
			if (m_subIdToMainId.find(id) != m_subIdToMainId.end())
			{
				return m_subIdToMainId[id];
			}

			// MainAssetならそのまま返す
			return id;
		}

		// 相互変換を行う関数
		UUID GetUUID(const std::filesystem::path& path) const
		{
			if (m_pathToUuid.find(path) == m_pathToUuid.end()) return UUID_NONE;

			return m_pathToUuid.at(path);
		}
		const std::filesystem::path& GetPath(UUID uuid) const
		{
			static std::filesystem::path errorPath{};

			if (m_uuidToPath.find(uuid) == m_uuidToPath.end()) return errorPath;

			return m_uuidToPath.at(uuid);
		}
		
		// 相互変換を行う関数
		AssetAux GetAux(UUID uuid) const
		{
			if (m_uuidToAux.find(uuid) == m_uuidToAux.end()) return {};

			return m_uuidToAux.at(uuid);
		}

		//------- ファイル操作関数 -------//

		// 名前変更
		void ReName(const std::filesystem::path& old, const std::filesystem::path& next);

		// 移動
		void Move(const std::filesystem::path& old, const std::filesystem::path& next);

		// 削除
		void Delete(const std::filesystem::path& path);

		// 新しくUUIDの生成を行う関数
		UUID GenerateUUID();

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------
		// UUIDとパスの登録をする関数
		void Register(UUID uuid, const std::filesystem::path& path)
		{
			// uuid->pathの変換表に追加
			m_uuidToPath[uuid] = path;

			// path->uuidの変換表に追加
			m_pathToUuid[path] = uuid;
		}
	};
}	// namespace REngine
