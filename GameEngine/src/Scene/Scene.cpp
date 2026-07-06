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

// 各管理クラス
#include "Scene/UpdatePipeline.h"
#include "Managers/UI/UIManager.h"
#include "Managers/3DManagers/Ray/RaySystem.h"

// システム
#include "System/ResourceManager.h"
#include "System/WindowManager.h"
#include "ComponentRegister.h"

#include "Debug/DebugManager.h"

// 各コンポーネント
#include "Components/ComponentBase.h"
#include "Components/World/Camera/CameraBase.h"
#include "Components/World/Transform/Transform.h"

#include "GameObject/GameObject.h"
#include "Renderer/Renderer.h"
#include "Scene/Transition/TransitionBase.h"
#include "Timer/GameTimer.h"
#include "SimpleMath.h"
#include "Input/MouseInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Scene::Scene(SceneManager* pSceneManager)
	: m_pSceneManager(pSceneManager)
	, m_updatePipeline		{ std::make_unique<UpdatePipeline>(this) }
	, m_componentRegister	{ std::make_unique<ComponentRegister>(m_updatePipeline.get()) }
	, m_defaultRenderTarget { std::make_unique<RenderTarget>() }
	, m_drawMainScreen{ true }
	, m_startPoint{ 0, 0 }
	, m_scale{ 1.0f, 1.0f }
	, m_play{ true }
{
	// レンダーターゲットの初期化
	m_defaultRenderTarget->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		WindowManager::Instance().GetWidth(),
		WindowManager::Instance().GetHeight()
	);

	// 初期化
	m_updatePipeline->Initialize();
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

	// 各管理クラスの更新
	m_updatePipeline->Update(gameTimer, m_play);
}

/// <summary>
/// 基底描画関数
/// </summary>
/// <param name="renderer"></param>
void Scene::BaseRender(Renderer& renderer)
{
	// メインカメラの描画
	RenderWithContext(
		{ GetMainCamera(), m_defaultRenderTarget.get(), WindowManager::Instance().GetBackGroundColor(),
			DrawFlag::World | 
			DrawFlag::UI | 
			DrawFlag::ColliderDebug | 
			DrawFlag::RectDebug
		},
		renderer
	);

	// 派生クラスの描画処理
	Render(renderer);
}

void Scene::BaseRenderOnScreen(Renderer& renderer)
{
	if (m_drawMainScreen)
	{
		// 描画
		renderer.Draw().Sprite()
			.Extend(m_scale)
			.Execute(m_defaultRenderTarget->GetShaderResourceView(), m_startPoint);
	}

	// 派生クラスの描画
	RenderOnScreen(renderer);
}

void Scene::RenderWithContext(const RenderContext& context, Renderer& renderer)
{
	auto* deviceContext = ResourceManager::Instance().GetResources()->GetD3DDeviceContext();

	// 描画を開始
	context.target->Begin(deviceContext);

	// クリア
	context.target->Clear(deviceContext, context.back);

	// Renderの開始
	renderer.Start(deviceContext);

	// 各行列の設定
	if (context.camera)
	{
		renderer.SetView(context.camera->GetView());
		renderer.SetProjection(context.camera->GetProj());
	}

	// Worldの描画
	if (context.flags & DrawFlag::World)
	{
		// 描画管理クラスの描画処理
		m_updatePipeline->DrawWorld(renderer);
	}

	// 色 (緑)
	static const DirectX::SimpleMath::Color color = { 0, 1, 0, 1 };

	// コライダーのデバッグ描画
	if (context.flags & DrawFlag::ColliderDebug && DebugManager::Instance().IsDrawColliderDebug())
	{
		m_updatePipeline->DrawColliders(renderer, color);
	}

	// 行列のリセット
	renderer.SetView(DirectX::SimpleMath::Matrix::Identity);
	renderer.SetProjection(DirectX::SimpleMath::Matrix::Identity);

	// UIの描画
	if (context.flags & DrawFlag::UI)
	{
		m_updatePipeline->DrawUI(renderer);
	}

	// RectTransformのデバッグ描画
	if (context.flags & DrawFlag::RectDebug && DebugManager::Instance().IsDrawRectTransform())
	{
		m_updatePipeline->DrawRects(renderer, color);
	}

	// Rendererの終了
	renderer.End();

	// 描画の終了
	context.target->End(deviceContext);
}

/// <summary>
/// 基底終了関数
/// </summary>
void Scene::BaseFinalize()
{
	Finalize();

	m_updatePipeline->Finalize();
}

GameObject* Scene::Generate(DirectX::SimpleMath::Vector3 position)
{
	// ポインタを作成
	GameObject* pObj = new GameObject(GameObject::CreateToken{});

	// オブジェクトのシーンに自身を設定
	pObj->SetScene(this);

	// オブジェクト管理クラスに追加
	m_updatePipeline->GetObjectManager()->AddObject(pObj);

	// Transformを追加
	pObj->AddComponent<Transform>();

	// 位置を設定
	pObj->GetComponent<Transform>()->SetLocalPosition(position);

	// 作成したポインタを返す
	return pObj;
}

Canvas* Scene::GenerateCanvas()
{
	return m_updatePipeline->GetUIManager()->CreateCanvas();
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
	m_updatePipeline->GetCameraManager()->SetMainCamera(camera);
}

// メインカメラ取得関数
CameraBase* Scene::GetMainCamera() const
{
	return m_updatePipeline->GetCameraManager()->GetMainCamera();
}

// Rayの衝突を調べる
bool Scene::RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask)
{
	return 
		RaySystem::RayCast(
		m_updatePipeline->GetPhysicsManager()->GetCollideManager()->GetAllColliders(),
		ray, max, hit, layerMask
	);
}

DirectX::SimpleMath::Vector2 Scene::GetMousePointOnMainScreen()
{
	return (Input::Mouse::GetScaledMousePoint() - m_startPoint) / m_scale;
}

// シーンの変更
void Scene::ChangeScene(const std::string& nextSceneName, std::unique_ptr<Transition::Base> outTrans, std::unique_ptr<Transition::Base> inTrans)
{
	m_pSceneManager->RequestSceneChange(nextSceneName, std::move(outTrans), std::move(inTrans));
}

// シーンの変更（演出なし）
void Scene::ChangeScene(const std::string& nextSceneName)
{
	m_pSceneManager->RequestSceneChange(nextSceneName);
}

ObjectManager* Scene::GetObjectManager() const
{
	return m_updatePipeline->GetObjectManager();
}

UIManager* Scene::GetUIManager() const
{
	return m_updatePipeline->GetUIManager();
}
