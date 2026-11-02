/**
 * @file    AssetDataBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   AssetとUUIDの双変換を行うクラス
 */

#pragma once

#include <filesystem>
#include <unordered_map>

#include "AuxFileRepository.h"
#include "AssetTypeManager.h"

namespace RHEngine
{
	/**
	 * @brief アセット本体とUUIDの双変換を行う
	 */
	class AssetDataBase
	{
	private:

		/** @brief ファイルへの読み書きをするインスタンス */
		AuxFileRepository m_auxFileRepository;

		/** @brief タイプ管理クラス */
		const AssetTypeManager& m_assetTypeManager;

		/** @brief uuidからファイルパスの変換マップ */
		std::unordered_map<UUID, std::filesystem::path> m_uuidToPath;

		/** @brief ファイルパスからuuidの変換マップ */
		std::unordered_map<std::filesystem::path, UUID> m_pathToUuid;

		/** @brief UUIDからAuxの変換マップ */
		std::unordered_map<UUID, AssetAux> m_uuidToAux;

		/** @brief サブアセットIDからメインアセットIDの変換マップ */
		std::unordered_map<UUID, UUID> m_subIdToMainId;

	public:

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param assetTypeManager アセットタイプマネージャー
		 */
		AssetDataBase(const AssetTypeManager& assetTypeManager)
			: m_assetTypeManager{ assetTypeManager }
		{
		}

		~AssetDataBase() = default;

		/// @}

		/**
		 * @brief ファイルを再帰的に走査してAssetAuxの生成と読み込みを行う
		 * @param root 読み込むルートフォルダ
		 */
		void ScanFile(const std::filesystem::path& root);

		/**
		 * @brief 1ファイルに対してAssetAuxの生成と読み込みを行う
		 * @param path 読み込み対象のパス
		 */
		void ScanOnceFile(const std::filesystem::path& path);

		/**
		 * @brief .auxファイルを保存する
		 * @param asset 保存するAssetAux
		 * @param path 保存対象のパス
		 */
		void SaveAux(AssetAux& asset, const std::filesystem::path& path);

		/**
		 * @brief MainAssetのUUIDを取得する
		 * @details 指定されたUUIDがMainAssetの物ならそのまま返される
		 *			SubAssetのものならSubAssetが所属するMainAssetのUUIDを返す
		 * @param id 対象のUUID
		 * @return MainAssetのUUID
		 */
		UUID GetMainUUID(UUID id);

		/**
		 * @brief パスからUUIDを取得する
		 * @param path 調べるパス
		 * @return 対応するUUID (未登録パスの場合UUID_NONE)
		 */
		UUID GetUUID(const std::filesystem::path& path) const;

		/**
		 * @brief UUIDからパスを取得する
		 * @param uuid 調べるUUID
		 * @return 対応するパス (未登録UUIDの場合デフォルトのpath)
		 */
		const std::filesystem::path& GetPath(UUID uuid) const;
		
		/**
		 * @brief UUIDからAssetAuxを取得する
		 * @param uuid 調べるUUID
		 * @return 対応するAssetAux (未登録UUIDの場合デフォルトAux)
		 */
		AssetAux GetAux(UUID uuid) const;

		/**
		 * @brief アセットファイルの名前を変更する
		 * @param old 変更前のパス
		 * @param next 変更後のパス
		 */
		void ReName(const std::filesystem::path& old, const std::filesystem::path& next);

		/**
		 * @brief アセットファイルの移動をする
		 * @param old 移動対象ファイルのパス
		 * @param next 移動するパス
		 */
		void Move(const std::filesystem::path& old, const std::filesystem::path& next);

		/**
		 * @brief アセットファイルを削除する関数
		 * @param path 削除対象のパス
		 */
		void Delete(const std::filesystem::path& path);

		/**
		 * @brief 新しいUUIDの生成をする
		 * @return 生成したUUID
		 */
		UUID GenerateUUID();

	private:

		/**
		 * @brief UUIDとパスの対応関係を登録する
		 * @param uuid 対象のUUID
		 * @param path 対象のパス
		 */
		void Register(UUID uuid, const std::filesystem::path& path);
	};
}	// namespace RHEngine
