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

//====================================================//
// インクルードファイル
//====================================================//
#include "GameObject/GameObject.h"

#include "Managers/ObjectManager.h"
#include "Managers/UI/UIManager.h"

class Scene;

class HierarchyWindow;
class InspectorWindow;
class ProjectWindow;

//====================================================//
// クラス宣言
//====================================================//
class EditGUI
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 所属先のシーン
	Scene* m_pScene;

	std::unique_ptr<HierarchyWindow> m_hierarchy;	// ヒエラルキーウィンドウ
	std::unique_ptr<InspectorWindow> m_inspector;	// インスペクターウィンドウ
	std::unique_ptr<ProjectWindow> m_project;		// プロジェクトウィンドウ

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	EditGUI(Scene* pScene);
	~EditGUI();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// ウィンドウの描画をする関数
	void DrawWindows();

	// ビューの描画をする関数
	void DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView);

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 画像の表示
	void DrawImage(ID3D11ShaderResourceView* img, DirectX::SimpleMath::Vector2 position);

	// シーンビュー描画の開始
	void StartSceneView();

	// ゲームビュー描画の開始
	void StartGameView();
};
