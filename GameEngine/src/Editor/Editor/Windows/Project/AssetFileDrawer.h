/**
 * @file    AssetFileDrawer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/08
 *
 * @brief   アセットファイルのGUI表示を行うクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <filesystem>

#include "Common/UUID.h"

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief アセットファイルをGUIに表示する
	 */
	class AssetFileDrawer
	{
	public:

		/**
		 * @brief 表示結果構造体
		 */
		struct Result
		{
			UUID select = UUID_NONE;		///< 選択しているアセットのUUID
			UUID doubleClick = UUID_NONE;	///< ダブルクリックされたアセットのUUID
		};

	private:

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

	public:

		/// @name ライフサイクル
		/// @{

		AssetFileDrawer(AssetManager& assetManager)
			: m_assetManager{ assetManager }
		{}

		~AssetFileDrawer() = default;

		/// @}

		/**
		 * @brief アセットファイルを表示する
		 * @param path 対象のファイルパス
		 * @param selected 現在選択中のアセットのUUID
		 * @return 表示結果
		 */
		Result DrawAssetFile(const std::filesystem::path& path, UUID selected);
	};
}	// namespace RHEngine

#endif
