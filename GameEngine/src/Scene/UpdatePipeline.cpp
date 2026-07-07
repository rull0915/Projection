//====================================================//
// ファイル名  : UpdatePipeline.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "UpdatePipeline.h"
#include "Scene/Scene.h"

#include "Debug/DebugManager.h"
#include "Debug/ColliderDebugRenderer.h"

#include "Input/MouseInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

UpdatePipeline::UpdatePipeline(Scene* pScene)
	: m_pScene{ pScene }
	, m_physicsManager		{ std::make_unique<PhysicsManager>() }
	, m_physicsManager2D	{ std::make_unique<PhysicsManager2D>() }
	, m_colEvent			{ std::make_unique<CollideEventSystem>() }
	, m_cameraManager		{ std::make_unique<CameraManager>() }
	, m_rendererManager		{ std::make_unique<RendererManager>() }
	, m_objectManager		{ std::make_unique<ObjectManager>() }
	, m_uiManager			{ std::make_unique<UIManager>(pScene) }
	, m_soundManager		{ std::make_unique<SoundManager>() }
{
}

void UpdatePipeline::Initialize()
{
	// カメラの初期化
	m_cameraManager->Initialize(m_pScene);
}

void UpdatePipeline::Update(const GameTimer& gameTimer, bool playing)
{
	// 各オブジェクトの更新
	m_objectManager->Update(gameTimer);

	// 各管理クラスの予約反映
	m_physicsManager->ReflectReserves();
	m_physicsManager2D->ReflectReserves();
	m_soundManager->ReflectReserves();

	if (playing)
	{
		// リジッドボディの更新
		m_physicsManager->Update(gameTimer.GetElapsedTime());

		// 2Dリジッドボディの更新
		m_physicsManager2D->Update(gameTimer.GetElapsedTime());

		// 衝突判定後の値の更新
		m_objectManager->AllReflectCache();

		// 衝突後関数の呼び出し
		m_colEvent->CallCollideFunctions(m_physicsManager->GetHitList());
		m_colEvent->CallCollideFunctions2D(m_physicsManager2D->GetHitList());

		// 音の更新
		m_soundManager->Update();

		// 各オブジェクトの遅延更新
		m_objectManager->LateUpdate(gameTimer);
	}

	// 削除予約を消す
	m_objectManager->RemoveDeadComponent();
	m_objectManager->RemoveDeadObject();

	// 描画管理クラスの更新
	m_rendererManager->Update();

	// UIの更新
	m_uiManager->Update(gameTimer);
	m_uiManager->CheckHitRay(m_pScene->GetMousePointOnMainScreen());
	m_uiManager->LateUpdate(gameTimer);
	m_uiManager->RemoveObjects();
}

void UpdatePipeline::DrawWorld(Renderer& renderer)
{
	// 描画管理クラスの描画処理
	m_rendererManager->DrawAll(renderer);
}

void UpdatePipeline::DrawUI(Renderer& renderer)
{
	m_uiManager->Draw(renderer);
}

void UpdatePipeline::DrawColliders(Renderer& renderer, DirectX::SimpleMath::Color color)
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

void UpdatePipeline::DrawRects(Renderer& renderer, DirectX::SimpleMath::Color color)
{
	m_uiManager->DebugDraw(renderer, color);
}

void UpdatePipeline::Finalize()
{
	// オブジェクトのリセット
	m_objectManager->Finalize();

	// 各マネージャーから全削除
	m_rendererManager->RemoveReserved();
	m_physicsManager->RemoveReserved();
	m_physicsManager->GetCollideManager()->ReflectReserves();

	m_physicsManager2D->RemoveReserved();
	m_physicsManager2D->GetCollideManager()->ReflectReserves();

	m_colEvent->ResetEvent();

	m_uiManager->Finalize();
	m_uiManager->Reset();
}
