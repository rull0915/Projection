//====================================================//
// ファイル名   : ProjectWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要 : プロジェクトウィンドウ
//
// 更新履歴 :
// 2026/07/06 新規作成
//====================================================//

#pragma once

#include <filesystem>
#include "Assets/Managers/AssetManager.h"
#include "Editor/Editor/SelectedOnGUI.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ProjectWindow
	{
	private:

		// アセットマネージャー
		AssetManager& m_assetManager;

		// 選択中オブジェクト
		SelectedOnGUI& m_selected;

		// 選択中パス
		std::filesystem::path m_selectedPath;

		// 出現フラグ
		bool m_openRenamePopup;
		bool m_openCreatePopup;

		// ポップアップに使用する文字列
		std::string m_popupStr;

		// リネーム対象のパス
		std::filesystem::path m_targetPath;

		// 作成対象のディレクトリ
		std::filesystem::path m_createDirectory;

		// 作成するアセットタイプ
		std::string m_createType;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ProjectWindow(AssetManager& am, SelectedOnGUI& selected)
			: m_assetManager{ am }
			, m_selected{ selected }
			, m_selectedPath{}
			, m_openRenamePopup{ false }
			, m_openCreatePopup{ false }
			, m_popupStr{ "" }
			, m_targetPath{}
			, m_createDirectory{}
		{}
		~ProjectWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 描画
		bool DrawProject();

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		bool StartProject();

		// 各リソースの描画
		void DrawResources();

		// ファイル構造の描画
		void DrawFileStructure(const std::filesystem::path& path);
	
		/// <summary>
		/// ファイル操作ポップアップの表示
		/// </summary>
		/// <param name="path">操作するファイルパス</param>
		/// <param name="isDirectory">ディレクトリかどうか</param>
		void DrawFileOperation(const std::filesystem::path& path, bool isDirectory);

		// 名前入力ポップアップの表示
		void DrawRenamePopup();

		// 新規作成ポップアップの表示
		void DrawCreatePopup();
	};
}	// namespace REngine
