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

#include "Common/PropertyObject.h"

class Scene;

class GameObject;
class Canvas;

class ObjectManager;
class UIManager;

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

	// 選択中のプロパティオブジェクト
	PropertyObject* m_selected;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	HierarchyWindow(Scene* pScene)
		: m_pScene{ pScene }
		, m_selected{ nullptr }
	{
	}
	~HierarchyWindow() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// ヒエラルキーの描画
	void DrawHierarchy();

	// 選択中オブジェクトのゲッター
	PropertyObject* GetSelected() const { return m_selected; }

private:

	// ヒエラルキー描画の開始
	void StartHierarchy();

	// GameObjectの表示関数
	void DrawGameObject(GameObject* object);

	// Canvasの表示関数
	void DrawCanvas(Canvas* canvas);

	// ObjectManagerから全Objectを描画する
	void DrawObjects(ObjectManager* objectManager);

	// UIManagerから全Objectを描画する
	void DrawObjects(UIManager* UIManager);
};
