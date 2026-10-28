/**
 * @file    HierarchyWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 *
 * @brief   ヒエラルキーウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

#include "Editor/Editor/SelectedOnGUI.h"

namespace RHEngine
{
	class Scene;
	class GameObject;
	class Canvas;
	class ObjectManager;

	/**
	 * @brief ヒエラルキーウィンドウ
	 */
	class HierarchyWindow
	{
	private:

		/** @brief 編集中のシーン */
		Scene* m_pScene;

		/** @brief 選択中のオブジェクト */
		SelectedOnGUI& m_selected;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 編集対象のシーン
		 * @param selected 選択オブジェクト
		 */
		HierarchyWindow(Scene* pScene, SelectedOnGUI& selected)
			: m_pScene{ pScene }
			, m_selected{ selected }
		{}

		~HierarchyWindow() = default;

		/// @}

		/**
		 * @brief ヒエラルキーウィンドウを表示する
		 */
		void DrawHierarchy();

	private:

		/**
		 * @brief ヒエラルキーウィンドウへの描画を開始する
		 */
		void StartHierarchy();

		/**
		 * @brief ヒエラルキーウィンドウにGameObjectを表示する
		 * @param pObject 対象のオブジェクト
		 */
		void DrawGameObject(GameObject* pObject);

		/**
		 * @brief ObjectManagerから全オブジェクトを表示する
		 * @param pObjectManager 対象のマネージャー
		 */
		void DrawObjects(ObjectManager* pObjectManager);

		/**
		 * @brief オブジェクトの選択を解除する
		 * @param root 対象のオブジェクト
		 * @return 解除したかどうか
		 */
		bool SelectCancel(GameObject* root);
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
