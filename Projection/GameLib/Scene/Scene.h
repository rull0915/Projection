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
#include "Transition/Transitions.h"
#include "GameLib/Common/Renderer/Renderer.h"

// 各マネージャー
#include "GameLib/GameObject/Managers/3DManagers/PhysicsManager.h"
#include "GameLib/GameObject/Managers/2DManagers/PhysicsManager2D.h"
#include "GameLib/GameObject/Managers/Renderer/RendererManager.h"
#include "GameLib/GameObject/Managers/CameraManager.h"
#include "GameLib/GameObject/Managers/ObjectManager.h"
#include "GameLib/GameObject/Managers/System/CollideEventSystem.h"

// UI
#include "GameLib/UIObject/Manager/UIManager.h"

//====================================================//
// 前方宣言
//====================================================//

class SceneManager;

//====================================================//
// クラス宣言
//====================================================//
class Scene
{
private:

	// シーンマネージャーへのポインタ
	SceneManager* m_pSceneManager;

	// 各マネージャーの実体
	PhysicsManager m_physicsManager;		// 3D物理
	PhysicsManager2D m_physicsManager2D;	// 2D物理
	CollideEventSystem m_colEvent;	// 衝突時のイベント

	CameraManager m_cameraManager;		// カメラ
	RendererManager m_rendererManager;	// 描画
	ObjectManager m_objectManager;		// オブジェクト

	UIManager m_uiManager;				// UI

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
	// 各マネージャーのゲッター
	PhysicsManager& GetPhysics() { return m_physicsManager; }
	PhysicsManager2D& GetPhysics2D() { return m_physicsManager2D; }

	CameraManager& GetCamera() { return m_cameraManager; }
	RendererManager& GetRenderer() { return m_rendererManager; }
	ObjectManager& GetObject(){ return m_objectManager; }

	UIManager& GetUI() { return m_uiManager; }

	// オブジェクトを生成する関数
	GameObject* Generate();

	// コンポーネントを登録する関数
	void RegisterComponent(BaseComponent* component);
	virtual void RegisterComponentOnDerived([[maybe_unused]] BaseComponent* component) {};

	// コンポーネントの登録を解除する関数
	void UnRegsisterComponent(BaseComponent* component);
	virtual void UnRegisterComponentOnDerived([[maybe_unused]] BaseComponent* component) {};

protected:

	// シーンの変更
	void ChangeScene(const std::string& nextSceneName, std::unique_ptr<SceneTransition> inTrans, std::unique_ptr<SceneTransition> outTrans);
};

inline GameObject* Scene::Generate()
{
	// GameObject派生クラスであれば

	// ポインタを作成
	GameObject* pObj = new GameObject();

	// オブジェクトのシーンに自身を設定
	pObj->SetScene(this);

	// オブジェクト管理クラスに追加
	m_objectManager.AddObject(pObj);

	// 作成したポインタを返す
	return pObj;
}

/// <summary>
/// コンポーネントを登録する関数
/// </summary>
/// <typeparam name="T">コンポーネントの型名</typeparam>
inline void Scene::RegisterComponent(BaseComponent* component)
{
	ComponentID id = MaskOperation(component->GetID(), MAIN_MASK);

	// マネージャーへコンポーネントを通知
	switch (id)
	{
	case ComponentID::Camera:
		m_cameraManager.SetMainCamera(static_cast<BaseCamera*>(component));
		break;
	case ComponentID::RigidBody:
		m_physicsManager.AddRigidBody(static_cast<RigidBody*>(component));
		break;
	case ComponentID::RigidBody2D:
		m_physicsManager2D.AddRigidBody(static_cast<RigidBody2D*>(component));
		break;
	case ComponentID::Collider:
		m_physicsManager.GetCollideManager().AddCollide(static_cast<BaseCollider*>(component));
		break;
	case ComponentID::Collider2D:
		m_physicsManager2D.GetCollideManager().AddCollide(static_cast<BaseCollider2D*>(component));
		break;
	case ComponentID::Renderer:
		m_rendererManager.AddRenderer(static_cast<BaseRenderer*>(component));
		break;
	default:
		break;
	}

	RegisterComponentOnDerived(component);
}

inline void Scene::UnRegsisterComponent(BaseComponent* component)
{
	ComponentID id = MaskOperation(component->GetID(), MAIN_MASK);

	switch (id)
	{
	case ComponentID::Camera:
		m_cameraManager.UnSetMainCamera(static_cast<BaseCamera*>(component));
		break;
	case ComponentID::RigidBody:
		m_physicsManager.RemoveRigidBody(static_cast<RigidBody*>(component));
		break;
	case ComponentID::RigidBody2D:
		m_physicsManager2D.RemoveRigidBody(static_cast<RigidBody2D*>(component));
		break;
	case ComponentID::Collider:
		m_physicsManager.GetCollideManager().RemoveCollide(static_cast<BaseCollider*>(component));
		m_colEvent.RemoveCollider(static_cast<BaseCollider*>(component));
		break;
	case ComponentID::Collider2D:
		m_physicsManager2D.GetCollideManager().RemoveCollide(static_cast<BaseCollider2D*>(component));
		m_colEvent.RemoveCollider2D(static_cast<BaseCollider2D*>(component));
		break;
	case ComponentID::Renderer:
		m_rendererManager.RemoveRenderer(static_cast<BaseRenderer*>(component));
		break;
	default:
		break;
	}
	
	UnRegisterComponentOnDerived(component);
}
