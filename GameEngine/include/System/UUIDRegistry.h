//====================================================//
// ファイル名   : UUIDRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/28
//
// 概要 : UUIDの生成を管理するクラス シングルトンにしプロジェクト全体で被らないように管理
//
// 更新履歴 :
// 2026/08/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_set>
#include "Common/UUID.h"

#include "Common/Random.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class UUIDRegistry
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 生成済みUUIDの一覧
		std::unordered_set<UUID> m_uuids;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------

	private:

		UUIDRegistry() = default;
		~UUIDRegistry() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

	public:

		// インスタンス取得関数
		static UUIDRegistry& Instance()
		{
			static UUIDRegistry instance;
			return instance;
		}

		// UUIDを生成する関数
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

		// UUIDを登録する関数
		void RegisterUUID(UUID uuid)
		{
			m_uuids.insert(uuid);
		}
	};
}
