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

	// 選択中のゲームオブジェクト
	GameObject* m_selected;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	EditGUI(Scene* pScene)
		: m_pScene{ pScene }
		, m_selected{ nullptr }
	{};
	~EditGUI() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ObjectManagerから全Objectを描画する
	void DrawObjects(ObjectManager* objectManager);

	// UIManagerから全Objectを描画する
	void DrawObjects(UIManager* UIManager);

	// GameObjectの情報をInspectorに表示する関数
	void DrawGameObjectOnInspector(GameObject* object);

	// 画像の表示
	void DrawImage(ID3D11ShaderResourceView* img, DirectX::SimpleMath::Vector2 position);

	// ヒエラルキー描画の開始
	void StartHierarchy();

	// インスペクター描画の開始
	void StartInspector();

	// シーンビュー描画の開始
	void StartSceneView();

	// ゲームビュー描画の開始
	void StartGameView();
	
	// ウィンドウの終了
	void EndWindow();

	// 選択中のオブジェクトを返す関数
	GameObject* GetSelected() const { return m_selected; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// GameObjectの表示関数
	void DrawGameObject(GameObject* object);

	// Canvasの表示関数
	void DrawCanvas(Canvas* canvas);

	// プロパティを一つ表示する関数
	bool DrawProperty(const Property* property);

	// コンポーネントの追加を表示する関数
	void DrawAddComponent(GameObject* object);

	// 値が変更されていた時の処理
	void OnChanged(ComponentBase* component);
};
