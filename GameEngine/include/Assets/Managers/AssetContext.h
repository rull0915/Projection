//====================================================//
// ファイル名   : AssetContext.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/08
//
// 概要 : アセットロード時に参照される仲介クラス
//
// 更新履歴 :
// 2026/10/08 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "AssetDataBase.h"
#include "AssetRegistry.h"
#include <functional>

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetContext
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		AssetDataBase& m_dataBase;

		AssetRegistry& m_registry;

		// UUIDからロードする関数オブジェクト
		std::function<UnTypeHandle(UUID)> m_loadFromUUID;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetContext(AssetDataBase& db, AssetRegistry& r, std::function<UnTypeHandle(UUID)> func)
			: m_dataBase{ db }
			, m_registry{ r }
			, m_loadFromUUID{ func }
		{
		}
		~AssetContext() = default;

		UnTypeHandle LoadFromUUID(UUID uuid) const { return m_loadFromUUID(uuid); }

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		AssetDataBase& GetDataBase() const { return m_dataBase; }
		AssetRegistry& GetRegistry() const { return m_registry; }
	};
}
