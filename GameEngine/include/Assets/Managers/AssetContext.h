/**
 * @file    AssetContext.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/08
 *
 * @brief   アセットロード時に参照される仲介クラス
 */

#pragma once

#include <functional>

#include "AssetDataBase.h"
#include "AssetRegistry.h"

namespace RHEngine
{
	/**
	 * @brief アセット読み書き時に参照されるコンテキスト
	 */
	class AssetContext
	{
	private:

		/** @brief データベース */
		AssetDataBase& m_dataBase;

		/** @brief レジストリ */
		AssetRegistry& m_registry;

		/** @brief UUIDからロードする関数 */
		std::function<UnTypeHandle(UUID)> m_loadFromUUID;

	public:

		/// @name ライフサイクル
		/// @{
		 
		/**
		 * @param db アセットデータベース
		 * @param r アセットレジストリ
		 * @param func UUIDからアセットを読み込む関数オブジェクト
		 */
		AssetContext(AssetDataBase& db, AssetRegistry& r, std::function<UnTypeHandle(UUID)> func)
			: m_dataBase{ db }
			, m_registry{ r }
			, m_loadFromUUID{ func }
		{
		}

		~AssetContext() = default;

		/// @}

		/**
		 * @brief UUID指定でロードを行う
		 * @param uuid 対象のUUID
		 * @return ロード完了アセットに対応するハンドル
		 */
		UnTypeHandle LoadFromUUID(UUID uuid) const { return m_loadFromUUID(uuid); }

		/// @name ゲッター
		/// @{

		/** @brief データベースの取得 */
		AssetDataBase& GetDataBase() const { return m_dataBase; }

		/** @brief レジストリの取得 */
		AssetRegistry& GetRegistry() const { return m_registry; }

		/// @}
	};
}	// namespace RHEngine
