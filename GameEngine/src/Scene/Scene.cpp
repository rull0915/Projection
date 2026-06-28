//====================================================//
// ファイル名  : Scene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/02/17
//
// 概要        :  シーンの基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Managers/ObjectManager.h"
#include "Managers/CameraManager.h"
#include "Managers/Renderer/RendererManager.h"
#include "Managers/2DManagers/PhysicsManager2D.h"
#include "Managers/2DManagers/Collider/CollideManager2D.h"
#include "Managers/3DManagers/PhysicsManager.h"
#include "Managers/3DManagers/Collider/CollideManager.h"
#include "Managers/UI/UIManager.h"
#include "Managers/3DManagers/Ray/RaySystem.h"

#include "Managers/System/CollideEventSystem.h"
#include "ComponentRegister.h"

#include "Debug/DebugManager.h"
#include "Debug/ColliderDebugRenderer.h"
#include <memory>
#include <string>
#include <utility>
#include "Components/ComponentBase.h"
#include "Components/World/Camera/CameraBase.h"
#include "Components/World/Transform/Transform.h"
#include "GameObject/GameObject.h"
#include "Renderer/Renderer.h"
#include "Scene/Transition/TransitionBase.h"
#include "Timer/GameTimer.h"
#include "SimpleMath.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Scene::Scene(SceneManager* pSceneManager)
	: m_pSceneManager(pSceneManager)
	, m_componentRegister	{ std::make_unique<ComponentRegister>(this) }
	, m_physicsManager		{ std::make_unique<PhysicsManager>() }
	, m_physicsManager2D	{ std::make_unique<PhysicsManager2D>() }
	, m_colEvent			{ std::make_unique<CollideEventSystem>() }
	, m_cameraManager		{ std::make_unique<CameraManager>() }
	, m_rendererManager		{ std::make_unique<RendererManager>() }
	, m_objectManager		{ std::make_unique<ObjectManager>() }
	, m_uiManager			{ std::make_unique<UIManager>(this) }
{
	m_cameraManager->Initialize(this);
}

// デストラクタ
Scene::~Scene()
{

}

/// <summary>
/// 基底初期化関数
/// </summary>
void Scene::BaseInitialize()
{
	Initialize();
}

void Scene::BaseUpdate(const GameTimer& gameTimer)
{
	// 派生クラスの更新
	Update(gameTimer);

	// 各オブジェクトの更新
	m_objectManager->Update(gameTimer);

	// リジッドボディの更新
	m_physicsManager->Update(gameTimer.GetElapsedTime());

	// 2Dリジッドボディの更新
	m_physicsManager2D->Update(gameTimer.GetElapsedTime());

	// 衝突判定後の値の更新
	m_objectManager->AllReflectCache();

	// 衝突後関数の呼び出し
	m_colEvent->CallCollideFunctions(m_physicsManager->GetHitList());
	m_colEvent->CallCollideFunctions2D(m_physicsManager2D->GetHitList());

	// 各オブジェクトの遅延更新
	m_objectManager->LateUpdate(gameTimer);

	// カメラの更新
	m_cameraManager->Update();

	// 削除予約を消す
	m_objectManager->RemoveDeadComponent();
	m_objectManager->RemoveDeadObject();

	// 描画管理クラスの更新
	m_rendererManager->Update();

	// UIの更新
	m_uiManager->Update(gameTimer);
	m_uiManager->LateUpdate(gameTimer);
	m_uiManager->RemoveObjects();
}

/// <summary>
/// 基底描画関数
/// </summary>
/// <param name="renderer"></param>
void Scene::BaseRender(Renderer& renderer)
{
	// メインカメラの行列を適用
	renderer.SetView(m_cameraManager->GetView());
	renderer.SetProjection(m_cameraManager->GetProj());

	// 描画管理クラスの描画処理
	m_rendererManager->DrawAll(renderer);

	// 派生クラスの描画処理
	Render(renderer);

	// 
	static const int color = 0x00FF00;

	// コライダーのデバッグ描画
	if (DebugManager::Instance().IsDrawColliderDebug())
	{
		// 3Dコライダー
		ColliderDebugRenderer::DebugDrawColliders(
			m_physicsManager->GetCollideManager()->GetAllColliders(),
			renderer, color, DebugManager::Instance().IsDrawColliderDebugAABB()
		);

		// 2Dコライダー
		ColliderDebugRenderer2D::DebugDrawColliders(
			m_physicsManager2D->GetCollideManager()->GetAllColliders(),
			renderer, color, DebugManager::Instance().IsDrawColliderDebugAABB()
		);
	}

	renderer.SetView(DirectX::SimpleMath::Matrix::Identity);
	renderer.SetProjection(DirectX::SimpleMath::Matrix::Identity);

	// UIの描画
	m_uiManager->Draw(renderer);

	// Rectのデバッグ描画
	if (DebugManager::Instance().IsDrawRectTransform())
	{
		m_uiManager->DebugDraw(renderer, color);
	}
}

/// <summary>
/// 基底終了関数
/// </summary>
void Scene::BaseFinalize()
{
	// オブジェクトのリセット
	m_objectManager->Finalize();
	
	// 各マネージャーから全削除
	m_rendererManager->RemoveReserved();
	m_physicsManager->RemoveReserved();
	m_physicsManager->GetCollideManager()->RemoveReserved();
	
	m_physicsManager2D->RemoveReserved();
	m_physicsManager2D->GetCollideManager()->RemoveReserved();

	m_colEvent->ResetEvent();

	m_uiManager->Finalize();
	m_uiManager->Reset();

	Finalize();
}

GameObject* Scene::Generate(DirectX::SimpleMath::Vector3 position)
{
	// ポインタを作成
	GameObject* pObj = new GameObject(GameObject::CreateToken{});

	// オブジェクトのシーンに自身を設定
	pObj->SetScene(this);

	// オブジェクト管理クラスに追加
	m_objectManager->AddObject(pObj);

	// Transformを追加
	pObj->AddComponent<Transform>();

	// 位置を設定
	pObj->GetComponent<Transform>()->SetLocalPosition(position);

	// 作成したポインタを返す
	return pObj;
}

Canvas* Scene::GenerateCanvas()
{
	return m_uiManager->CreateCanvas();
}

void Scene::RegisterComponent(ComponentBase* component)
{
	// 各マネージャーへ通知
	m_componentRegister->RegisterComponent(component);

	// 派生クラスに通知
	RegisterComponentOnDerived(component);
}

void Scene::UnRegsisterComponent(ComponentBase * component)
{
	// 各マネージャーへ通知
	m_componentRegister->UnRegisterComponent(component);

	// 派生クラスに通知
	UnRegisterComponentOnDerived(component);
}

// メインカメラ設定関数
void Scene::SetMainCamera(CameraBase* camera)
{
	m_cameraManager->SetMainCamera(camera);
}

// メインカメラ取得関数
CameraBase* Scene::GetMainCamera() const
{
	return m_cameraManager->GetMainCamera();
}

// Rayの衝突を調べる
bool Scene::RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask)
{
	return 
		RaySystem::RayCast(
		m_physicsManager->GetCollideManager()->GetAllColliders(),
		ray, max, hit, layerMask
	);
}

void Scene::ChangeScene(const std::string& nextSceneName, std::unique_ptr<Transition::Base> outTrans, std::unique_ptr<Transition::Base> inTrans)
{
	m_pSceneManager->RequestSceneChange(nextSceneName, std::move(outTrans), std::move(inTrans));
}

// シーンの変更
void Scene::ChangeScene(const std::string& nextSceneName)
{
	m_pSceneManager->RequestSceneChange(nextSceneName);
}
