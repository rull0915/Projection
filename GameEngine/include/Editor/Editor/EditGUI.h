/**
 * @file    EditGUI.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/05
 *
 * @brief   エディットのGUIを表示するクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <memory>
#include <functional>
#include <d3d11.h>

#include "SelectedOnGUI.h"
#include "SceneCamera.h"

namespace RHEngine
{
	class Scene;
	class AssetManager;

	class HierarchyWindow;
	class InspectorWindow;
	class ProjectWindow;
	class ToolBarWindow;
	class ImageWindow;
	class PerformanceWindow;

	/**
	 * @brief GUIのウィンドウを管理する
	 */
	class EditGUI
	{
	private:

		/** @brief 編集中のシーン */
		Scene* m_pScene;

		/// @name ウィンドウ
		/// @details ウィンドウを隠蔽するためにunique_ptrを使用する
		/// @{

		std::unique_ptr<ToolBarWindow> m_toolbar;			///< ツールバーウィンドウ
		std::unique_ptr<HierarchyWindow> m_hierarchy;		///< ヒエラルキーウィンドウ
		std::unique_ptr<InspectorWindow> m_inspector;		///< インスペクターウィンドウ
		std::unique_ptr<ProjectWindow> m_project;			///< プロジェクトウィンドウ
		std::unique_ptr<ImageWindow> m_images;				///< 画像用ウィンドウ
		std::unique_ptr<PerformanceWindow> m_performance;	///< パフォーマンスウィンドウ

		/// @}

		/** @brief アセットマネージャー */
		AssetManager& m_assetmanager;

		/**
		 * @brief 選択オブジェクトの管理クラス
		 * @details ヒエラルキーウィンドウ、プロジェクトウィンドウで選択し、選択されているものをインスペクターウィンドウで表示する
		 */
		SelectedOnGUI m_selected;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene シーンポインタ
		 * @param assetManager アセットマネージャー
		 * @param playFunc 再生する関数オブジェクト
		 * @param stopFunc 停止する関数オブジェクト
		 */
		EditGUI(Scene* pScene, AssetManager& assetManager, std::function<void()> playFunc, std::function<void()> stopFunc);
		~EditGUI();

		/// @}

		/**
		 * @brief 選択オブジェクトのリセットを行う
		 */
		void Reset();

		/**
		 * @brief メインウィンドウの描画を行う
		 * @details Hierarchy, Inspector, Projectの表示
		 */
		void DrawWindows();

		/**
		 * @brief シーンビューを表示する
		 * @param sceneView シーンビューテクスチャ
		 * @param pSceneViewCamera シーンビューのカメラ
		 * @return シーンビューカメラを動かせる状態かどうか
		 */
		bool DrawSceneView(ID3D11ShaderResourceView* sceneView, SceneCamera* pSceneViewCamera);

		/**
		 * @brief ゲームビューを表示する
		 * @param gameView ゲームビューテクスチャ
		 * @return ゲームビューウィンドウがクリックされたかどうか
		 */
		bool DrawGameView(ID3D11ShaderResourceView* gameView);

		/**
		 * @brief ツールバーを表示する
		 * @param engineLogo ゲームエンジンのロゴテクスチャ
		 * @param isPlaying 再生中かどうか
		 */
		void DrawToolBar(ID3D11ShaderResourceView* engineLogo, bool isPlaying);

		/**
		 * @brief ドッキング領域を表示する
		 */
		void DrawDockingArea();

		/**
		 * @brief パフォーマンスウィンドウを表示する
		 */
		void DrawPerformanceWindow();
	};
} // namespace RHEngine

#endif // ENGINE_GUI
