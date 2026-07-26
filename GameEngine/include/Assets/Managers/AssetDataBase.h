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

		// 相互変換のための対応表

		// uuidからファイルパス
		std::unordered_map<UUID, std::wstring> m_uuidToPath;

		// ファイルパスからuuid
		std::unordered_map<std::wstring, UUID> m_pathToUuid;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetDataBase() = default;
		~AssetDataBase() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ファイルを走査しauxの生成を行う関数
		void ScanFile(const std::filesystem::path& root, const AssetTypeManager& typeManager);

		// 相互変換を行う関数
		UUID GetUUID(const std::wstring& path)
		{
			return m_pathToUuid.at(path);
		}
		const std::wstring& GetPath(UUID uuid)
		{
			return m_uuidToPath.at(uuid);
		}

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// 新しくUUIDの生成を行う関数
		UUID GenerateUUID();

		// UUIDとパスの登録をする関数
		void Register(UUID uuid, const std::wstring& path)
		{
			// uuid->pathの変換表に追加
			m_uuidToPath[uuid] = path;

			// path->uuidの変換表に追加
			m_pathToUuid[path] = uuid;
		}
	};
}	// namespace REngine
