/**
 * @file    ProjectWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 *
 * @brief   プロジェクトウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

#include <filesystem>

#include "Common/UUID.h"
#include "Assets/Managers/AssetManager.h"
#include "Editor/Editor/SelectedOnGUI.h"
#include "ProjectWindowPopUp.h"
#include "AssetFileDrawer.h"

namespace RHEngine
{
	/**
	 * @brief プロジェクトウィンドウ
	 */
	class ProjectWindow
	{
	private:

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/** @brief 選択中のオブジェクト*/
		SelectedOnGUI& m_selected;

		/** @brief 選択中アセットのUUID */
		UUID m_selectedID;

		/** @brief ポップアップ表示*/
		ProjectWindowPopUp m_popUp;

		/** @brief アセットファイル表示*/
		AssetFileDrawer m_assetFileDrawer;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager 
		 * @param selected 選択オブジェクト
		 */
		ProjectWindow(AssetManager& assetManager, SelectedOnGUI& selected)
			: m_assetManager{ assetManager }
			, m_selected{ selected }
			, m_selectedID{}
			, m_popUp{ assetManager }
			, m_assetFileDrawer{ assetManager }
		{}

		~ProjectWindow() = default;

		/// @}

		/**
		 * @brief プロジェクトウィンドウを表示する
		 */
		void DrawProject();

	private:

		/**
		 * @brief プロジェクトウィンドウへの描画を開始する
		 */
		void StartProject();

		/**
		 * @brief 各リソースを描画する
		 */
		void DrawResources();

		/**
		 * @brief ファイル構造を再帰的に表示する
		 * @param path ルートファイルパス
		 */
		void DrawFileStructure(const std::filesystem::path& path);
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
