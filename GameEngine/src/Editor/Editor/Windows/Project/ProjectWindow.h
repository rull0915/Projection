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

#ifdef ENGINE_GUI

#include <filesystem>
#include "Assets/Managers/AssetManager.h"
#include "Editor/Editor/SelectedOnGUI.h"
#include "ProjectWindowPopUp.h"
#include "AssetFileDrawer.h"

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

		// ポップアップ管理クラス
		ProjectWindowPopUp m_popUp;

		// アセットファイル表示クラス
		AssetFileDrawer m_assetFileDrawer;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ProjectWindow(AssetManager& am, SelectedOnGUI& selected)
			: m_assetManager{ am }
			, m_selected{ selected }
			, m_selectedPath{}
			, m_popUp{ am }
			, m_assetFileDrawer{ am }
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
	};
}	// namespace REngine

#endif // ENGINE_GUI
