//====================================================//
// ファイル名   : HierarchyWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要 : ヒエラルキーウィンドウ
//
// 更新履歴 :
// 2026/07/06 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

#include "Editor/Editor/SelectedOnGUI.h"

namespace RHEngine
{
	class Scene;

	class GameObject;
	class Canvas;

	class ObjectManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class HierarchyWindow
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 所属先のシーン
		Scene* m_pScene;

		// 選択中オブジェクト
		SelectedOnGUI& m_selected;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		HierarchyWindow(Scene* pScene, SelectedOnGUI& selected)
			: m_pScene{ pScene }
			, m_selected{ selected }
		{}
		~HierarchyWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ヒエラルキーの描画
		bool DrawHierarchy();

	private:

		// ヒエラルキー描画の開始
		bool StartHierarchy();

		// GameObjectの表示関数
		void DrawGameObject(GameObject* object);

		// ObjectManagerから全Objectを描画する
		void DrawObjects(ObjectManager* objectManager);

		// 選択解除をする関数
		bool SelectCancel(GameObject* root);
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
