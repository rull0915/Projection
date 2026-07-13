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

class Scene;

class HierarchyWindow;
class InspectorWindow;
class ProjectWindow;
class InfoWindow;

//====================================================//
// クラス宣言
//====================================================//
class EditGUI
{
public:
	// ウィンドウタイプ
	enum class WindowType
	{
		None,
		Hieraychy,
		Inspector,
		Project,
		Info,
		GameView,
		SceneView,
	};

private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 所属先のシーン
	Scene* m_pScene;

	std::unique_ptr<HierarchyWindow> m_hierarchy;	// ヒエラルキーウィンドウ
	std::unique_ptr<InspectorWindow> m_inspector;	// インスペクターウィンドウ
	std::unique_ptr<ProjectWindow> m_project;		// プロジェクトウィンドウ
	std::unique_ptr<InfoWindow> m_info;				// インフォウィンドウ

	// クリックされているウィンドウ
	WindowType m_nowType;

	// シーンビューの描画設定
	unsigned char m_sceneDrawSetting;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	EditGUI(Scene* pScene, std::function<void()> playFunc);
	~EditGUI();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Reset();

	// ウィンドウの描画をする関数
	void DrawWindows();

	// ビューの描画をする関数
	void DrawViews(ID3D11ShaderResourceView* sceneView, ID3D11ShaderResourceView* gameView);

	// ウィンドウをセットする関数
	void SetWindowType(WindowType type) { m_nowType = type; }

	// ウィンドウを取得する関数
	WindowType GetWindowType() { return m_nowType; }

	// 描画設定を取得する関数
	unsigned char GetDrawSetting() const { return m_sceneDrawSetting; }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 画像の表示
	void DrawImage(ID3D11ShaderResourceView* img);

	// シーンビュー描画の開始
	void StartSceneView();

	// ゲームビュー描画の開始
	void StartGameView();
};
