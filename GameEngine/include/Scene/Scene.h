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
#include "GameObject/GameObject.h"

#include "Transition/TransitionBase.h"
#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"

//====================================================//
// 前方宣言
//====================================================//

class SceneManager;
class ComponentRegister;

// 各管理クラス
class ObjectManager;
class CameraManager;
class PhysicsManager;
class PhysicsManager2D;
class RendererManager;
class UIManager;
class CollideEventSystem;

class Renderer;
class CameraBase;

class Canvas;

//====================================================//
// クラス宣言
//====================================================//
class Scene
{
	// friend指定
	friend ComponentRegister;

private:

	// シーンマネージャーへのポインタ
	SceneManager* m_pSceneManager;

	// コンポーネント登録システム
	std::unique_ptr<ComponentRegister> m_componentRegister;

	// 各マネージャーの実体
	std::unique_ptr<PhysicsManager> m_physicsManager;		// 3D物理
	std::unique_ptr<PhysicsManager2D> m_physicsManager2D;	// 2D物理
	std::unique_ptr<CollideEventSystem> m_colEvent;			// 衝突時のイベント

	std::unique_ptr<CameraManager> m_cameraManager;		// カメラ
	std::unique_ptr<RendererManager> m_rendererManager;	// 描画
	std::unique_ptr<ObjectManager> m_objectManager;		// オブジェクト

	std::unique_ptr<UIManager> m_uiManager;				// UI

public:

	// コンストラクタ
	Scene(SceneManager* pSceneManager);

	// デストラクタ
	virtual ~Scene();

	// 初期化処理
	void BaseInitialize();
	virtual void Initialize() = 0;

	// 更新処理
	void BaseUpdate(const GameTimer& gameTimer);
	virtual void Update(const GameTimer& gameTimer) = 0;

	// 描画処理
	void BaseRender(Renderer& renderer);
	virtual void Render(Renderer& renderer) = 0;

	// 終了処理
	void BaseFinalize();
	virtual void Finalize() = 0;

public:

	// オブジェクトを生成する関数
	GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });
	// キャンバスを生成する関数
	Canvas* GenerateCanvas();

	// コンポーネントを登録する関数
	void RegisterComponent(ComponentBase* component);

	// コンポーネントの登録を解除する関数
	void UnRegsisterComponent(ComponentBase* component);

	// 派生クラスへ通知する用
	virtual void RegisterComponentOnDerived([[maybe_unused]] ComponentBase* component) {};
	virtual void UnRegisterComponentOnDerived([[maybe_unused]] ComponentBase* component) {};

	// メインカメラをセットする関数
	void SetMainCamera(CameraBase* camera);

	// メインカメラを取得する関数
	CameraBase* GetMainCamera() const;

	// RayCast関数
	bool RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);

protected:

	// シーンの変更
	void ChangeScene(const std::string& nextSceneName, std::unique_ptr<Transition::Base> outTrans, std::unique_ptr<Transition::Base> inTrans);
	void ChangeScene(const std::string& nextSceneName);
};
