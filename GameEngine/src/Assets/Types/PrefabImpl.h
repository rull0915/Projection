/**
 * @file    PrefabImpl.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/28
 *
 * @brief   プレハブアセットの実装
 * @details jsonを隠蔽するためにimplを経由させる
 */

#pragma once

#include <fstream>
#include <filesystem>
#include <cassert>

#include "Thirdparty/nlohmann/json.hpp"

namespace RHEngine
{
	/**
	 * @brief プレハブアセットの実装
	 */
	class PrefabImpl
	{
	private:

		/** @brief json本体 */
		nlohmann::json m_json;

		/** @brief World空間のPrefabかどうか */
		bool m_isWorld;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param path jsonファイルのパス
		 */
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

		/**
		 * @param j 対応するjson
		 */
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

		/** @brief jsonを取得する */
		const nlohmann::json& GetJson() const
		{
			return m_json; 
		}

		/** @brief World空間のプレハブかどうかを取得する */
		bool IsWorldObject() const { return m_isWorld; }
	};
}	// namespace RHEngine
