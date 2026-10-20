//====================================================//
// ファイル名   : PrefabImpl.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/28
//
// 概要 : 
//
// 更新履歴 :
// 2026/07/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <fstream>
#include "Thirdparty/nlohmann/json.hpp"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class PrefabImpl
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// json本体
		nlohmann::json m_json;

		// World空間かどうか
		bool m_isWorld;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		PrefabImpl(const std::filesystem::path& path)
			: m_json{}
			, m_isWorld{ true }
		{
			std::ifstream ifs(path);

			// 開けていたら
			if (ifs.is_open())
			{
				// jsonへ
				ifs >> m_json;
			}

			// 閉じる
			ifs.close();

			if (m_json.contains("IsWorld") && !m_json["IsWorld"])
			{
				m_isWorld = false;
			}
		}

		PrefabImpl(const nlohmann::json& j)
			: m_json( j )
			, m_isWorld{ true }
		{
			if (m_json.contains("IsWorld") && !m_json["IsWorld"])
			{
				m_isWorld = false;
			}

			assert(m_json.is_object());
		}
		~PrefabImpl() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// jsonのゲッター
		const nlohmann::json& GetJson() const
		{
			return m_json; 
		}

		// World空間オブジェクトかどうか
		bool IsWorldObject() const { return m_isWorld; }
	};
}	// namespace RHEngine
