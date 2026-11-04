/**
 * @file    ProjectWindowPopUp.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/08
 *
 * @brief   プロジェクトウィンドウが扱うポップアップを管理するクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <string>
#include <filesystem>

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief プロジェクトウィンドウが扱うポップアップを管理する
	 */
	class ProjectWindowPopUp
	{
	private:

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/** @brief 選択中のパス */
		std::filesystem::path m_selectedPath;

		/** @brief 名前変更ポップアップが出現するかどうか */
		bool m_openRenamePopup;

		/** @brief 新規作成ポップアップが出現するかどうか */
		bool m_openCreatePopup;

		/** @brief ポップアップの文字列 */
		std::string m_popupStr;

		/** @brief 名前変更対象のパス */
		std::filesystem::path m_targetPath;

		/** @brief 新規作成対象のディレクトリ */
		std::filesystem::path m_createDirectory;

		/** @brief 作成するアセットタイプ名 */
		std::string m_createType;

	public:

		/// @name ライフサイクル
		/// @{

		ProjectWindowPopUp(AssetManager& assetManager)
			: m_assetManager{ assetManager }
			, m_selectedPath{}
			, m_openRenamePopup{ false }
			, m_openCreatePopup{ false }
			, m_popupStr{}
			, m_targetPath{}
			, m_createDirectory{}
			, m_createType{}
		{}

		~ProjectWindowPopUp() = default;

		/// @}

		/**
		 * @brief ファイル操作ポップアップの表示
		 * @param path 操作するファイルパス
		 * @param isDirectory ディレクトリかどうか
		 */
		void DrawFileOperation(const std::filesystem::path& path, bool isDirectory);

		/**
		 * @brief 名前変更ポップアップを開く
		 * @param path 変更対象のパス
		 */
		void OpenRenamePopUp(const std::filesystem::path& path);

		/**
		 * @brief 新規作成ポップアップを開く
		 * @param path 作成対象のディレクトリ
		 */
		void OpenCreatePopUp(const std::filesystem::path& path);

		/**
		 * @brief 名前変更ポップアップを描画する
		 */
		void DrawRenamePopup();

		/**
		 * @brief 新規作成ポップアップを描画する
		 */
		void DrawCreatePopup();
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
