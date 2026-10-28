/**
 * @file    UUIDRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/28
 *
 * @brief   UUIDの生成を管理するクラス
 */

#pragma once

#include <unordered_set>

#include "Common/UUID.h"
#include "Common/Random.h"

namespace RHEngine
{
	/**
	 * @brief UUIDの生成を管理する
	 * @details シングルトンにすることでプロジェクト全体でUUIDの重複が起こらないようにする
	 * @TODO まだファイルロード時に被る可能性があるのでファイルに保存されているUUIDをプロジェクト開始時に全て登録する
	 */
	class UUIDRegistry
	{
	private:

		/** @brief 生成済みのUUID一覧 */
		std::unordered_set<UUID> m_uuids;

	private:

		/// @name ライフサイクル
		/// @{

		UUIDRegistry() = default;

		~UUIDRegistry() = default;

		/// @}

	public:
		/**
		 * @brief インスタンスを取得する
		 * @return 静的なインスタンス
		 */
		static UUIDRegistry& Instance()
		{
			static UUIDRegistry instance;
			return instance;
		}

		/**
		 * @brief 新しいUUIDを生成する
		 * @return 生成したUUID
		 */
		UUID GenerateUUID()
		{
			UUID uuid = Random::Get<UUID>(0, std::numeric_limits<UUID>::max());

			// リストにないUUIDが生成されるまでループ
			while (m_uuids.contains(uuid))
			{
				// ランダム生成
				uuid = Random::Get<UUID>(0, std::numeric_limits<UUID>::max());
			}

			// UUIDを返す
			return uuid;
		}

		/**
		 * @brief 生成済みのUUIDを登録する
		 */
		void RegisterUUID(UUID uuid)
		{
			m_uuids.insert(uuid);
		}
	};
}	// namespace RHEngine
