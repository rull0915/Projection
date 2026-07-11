//====================================================//
// ファイル名   : Scene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/02/06
//
// 概要 : Sceneの基底クラス
//
// 更新履歴 :
// 2026/02/06 新規作成
// 2026/05/04 コンポーネントのマネージャーを所持するように変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>

#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"
#include "UpdateMode.h"

#include "ObjectFactory.h"
#include "ComponentRegister.h"
#include "ObjectFinder.h"
#include "MainScreen.h"

//====================================================//
// 前方宣言
//====================================================//

class Renderer;
class CameraBase;
class Canvas;

class SceneRenderer;
class UpdatePipeline;
class ObjectManager;
class UIManager;

//====================================================//
// クラス宣言
//====================================================//
class Scene final
{
private:

	// マネージャー管理をするクラス
	std::unique_ptr<UpdatePipeline> m_updatePipeline;

	// コンポーネント登録システム
	std::unique_ptr<ComponentRegister> m_componentRegister;

	// オブジェクト検索
	std::unique_ptr<ObjectFinder> m_objectFinder;

	// シーン描画
	std::unique_ptr<SceneRenderer> m_sceneRenderer;

	// ファクトリ
	std::unique_ptr<ObjectFactory> m_objectFactory;

	// メインスクリーン
	std::unique_ptr<MainScreen> m_mainScreen;

	// 更新状態
	UpdateMode m_updateMode;

public:

	// コンストラクタ
	Scene();

	// デストラクタ
	virtual ~Scene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(const GameTimer& gameTimer);

	// 描画処理
	void Render(Renderer& renderer);

	// スクリーン本体への描画処理
	void RenderOnScreen(Renderer& renderer);

	// 終了処理
	void Finalize();

	// 全オブジェクトの削除関数
	void ResetObjects();

public:

	// コンポーネントを登録する関数
	void RegisterComponent(ComponentBase* component);

	// コンポーネントの登録を解除する関数
	void UnRegsisterComponent(ComponentBase* component);

	// RayCast関数
	bool RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// メインカメラを取得する関数
	CameraBase* GetMainCamera() const;

	// 管理クラス
	ObjectManager* GetObjectManager() const;
	UIManager* GetUIManager() const;

	// コンポーネント管理者
	ComponentRegister* GetComponentRegister() const { return m_componentRegister.get(); }

	// オブジェクトファインダー
	ObjectFinder* GetObjectFinder() const { return m_objectFinder.get(); }

	// パイプライン
	UpdatePipeline* GetPipeline() const { return m_updatePipeline.get(); }

	// ファクトリ
	ObjectFactory* GetFactory() const { return m_objectFactory.get(); }

	// メインスクリーン
	MainScreen* GetMainScreen() const { return m_mainScreen.get(); }
	 
	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	
	// メインカメラをセットする関数
	void SetMainCamera(CameraBase* camera);

	// 更新モード
	void SetPlayFlag(UpdateMode mode) { m_updateMode = mode; }
};
