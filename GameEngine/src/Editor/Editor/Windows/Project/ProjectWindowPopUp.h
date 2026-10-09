//====================================================//
// ファイル名   : ProjectWindowPopUp.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/08
//
// 概要 : プロジェクトウィンドウが扱うポップアップを管理するクラス
//
// 更新履歴 :
// 2026/10/08 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <filesystem>

namespace REngine
{
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ProjectWindowPopUp
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// アセットマネージャー
		AssetManager& m_assetManager;

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
		ProjectWindowPopUp(AssetManager& am)
			: m_assetManager{ am }
			, m_selectedPath{}
			, m_openRenamePopup{ false }
			, m_openCreatePopup{ false }
			, m_popupStr{}
			, m_targetPath{}
			, m_createDirectory{}
			, m_createType{}
		{}
		~ProjectWindowPopUp() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		/// <summary>
		/// ファイル操作ポップアップの表示
		/// </summary>
		/// <param name="path">操作するファイルパス</param>
		/// <param name="isDirectory">ディレクトリかどうか</param>
		void DrawFileOperation(const std::filesystem::path& path, bool isDirectory);

		// 名前変更ポップアップの出現
		void OpenRenamePopUp(const std::filesystem::path& path);

		// 新規作成ポップアップの出現
		void OpenCreatePopUp(const std::filesystem::path& path);

		// 名前変更ポップアップの描画
		void DrawRenamePopup();

		// 新規作成ポップアップの描画
		void DrawCreatePopup();
	};
}
