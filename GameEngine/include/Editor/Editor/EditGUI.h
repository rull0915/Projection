//====================================================//
// ファイル名   : EditGUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要 : エディットのGUIを表示するクラス
//
// 更新履歴 :
// 2026/07/05 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

#include "SelectedOnGUI.h"
#include "SceneCamera.h"

//====================================================//
// インクルードファイル
//====================================================//

namespace RHEngine
{
	class Scene;

	class HierarchyWindow;
	class InspectorWindow;
	class ProjectWindow;
	class ToolBarWindow;
	class ImageWindow;
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class EditGUI
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 所属先のシーン
		Scene* m_pScene;

		std::unique_ptr<ToolBarWindow> m_toolbar;			// ツールバーウィンドウ
		std::unique_ptr<HierarchyWindow> m_hierarchy;	// ヒエラルキーウィンドウ
		std::unique_ptr<InspectorWindow> m_inspector;	// インスペクターウィンドウ
		std::unique_ptr<ProjectWindow> m_project;		// プロジェクトウィンドウ
		std::unique_ptr<ImageWindow> m_images;			// 画像用ウィンドウ

		// シーンビューの描画設定
		unsigned char m_sceneDrawSetting;

		// アセットマネージャー
		AssetManager& m_assetmanager;

		// 選択中オブジェクト
		SelectedOnGUI m_selected;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		EditGUI(Scene* pScene, AssetManager& assetManager, std::function<void()> playFunc, std::function<void()> stopFunc);
		~EditGUI();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Reset();

		// ウィンドウの描画をする関数
		void DrawWindows();

		// シーンビューの描画をする関数
		bool DrawSceneView(ID3D11ShaderResourceView* sceneView, SceneCamera* sceneViewCamera);

		bool DrawGameView(ID3D11ShaderResourceView* gameView);

		// 描画設定を取得する関数
		unsigned char GetDrawSetting() const { return m_sceneDrawSetting; }

		// スタイルエディタウィンドウを表示する関数
		void DrawStyleEditor();

		// ツールバーを表示する関数
		void DrawToolBar(ID3D11ShaderResourceView* engineLogo, bool isPlaying);

		// ドッキング領域を描画する関数
		void DrawDockingArea();

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	};
} // namespace RHEngine

#endif // ENGINE_GUI
