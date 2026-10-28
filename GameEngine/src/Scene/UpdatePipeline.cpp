/**
 * @file    UpdatePipeline.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/05
 */

#include "pch.h"
#include "UpdatePipeline.h"

#include "Scene/Scene.h"
#include "Input/MouseInput.h"

namespace RHEngine
{
	UpdatePipeline::UpdatePipeline(Scene* pScene)
		: m_pScene{ pScene }
		, m_physicsManager{ std::make_unique<PhysicsManager>() }
		, m_physicsManager2D{ std::make_unique<PhysicsManager2D>() }
		, m_colEvent{ std::make_unique<CollideEventSystem>() }
		, m_cameraManager{ std::make_unique<CameraManager>() }
		, m_rendererManager{ std::make_unique<RendererManager>() }
		, m_lightManager{ std::make_unique<LightManager>() }
		, m_objectManager{ std::make_unique<ObjectManager>() }
		, m_uiManager{ std::make_unique<UIManager>(pScene) }
		, m_soundManager{ std::make_unique<SoundManager>() }
	{}

	void UpdatePipeline::Update(const GameTimer& gameTimer, UpdateMode mode)
	{
		// ポーズの場合何もしない
		if (mode == UpdateMode::Pause) return;

		bool playing = mode == UpdateMode::Play;

		// 各オブジェクトの更新
		m_objectManager->Update(gameTimer, playing);

		m_rendererManager->ReflectReserves();
		m_lightManager->ReflectReserves();
		m_uiManager->ReflectReserves();
		m_cameraManager->ReflectReserves();

		if (playing)
		{
			// 各管理クラスの予約反映
			m_physicsManager->ReflectReserves();
			m_physicsManager2D->ReflectReserves();
			m_soundManager->ReflectReserves();

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
		}

		// 各オブジェクトの遅延更新
		m_objectManager->LateUpdate(gameTimer, playing);

		// 削除予約を消す
		m_objectManager->RemoveDeadComponent();
		m_objectManager->RemoveDeadObject();

		// UIの更新
		if (playing)
		{
			// To Refactor
			m_uiManager->CheckHitRay(Input::Mouse::GetMousePoint());
			m_uiManager->CheckEvent();
		}
	}

	void UpdatePipeline::DrawWorld(Renderer& renderer)
	{
		// 描画管理クラスの描画処理
		m_rendererManager->DrawAll(renderer);
	}

	void UpdatePipeline::DrawUI(Renderer& renderer)
	{
		m_uiManager->ReflectReserves();

		m_uiManager->Draw(renderer);
	}

	void UpdatePipeline::DrawRects(Renderer& renderer, DirectX::SimpleMath::Color color)
	{
		m_uiManager->ReflectReserves();

		m_uiManager->DebugDraw(renderer, color);
	}

	void UpdatePipeline::Finalize()
	{
		// オブジェクトのリセット
		m_objectManager->Finalize();

		m_uiManager->Reset();

		// 各マネージャーから全削除
		m_rendererManager->RemoveReserved();
		m_lightManager->RemoveReserved();
		m_physicsManager->RemoveReserved();
		m_physicsManager->GetCollideManager()->ReflectReserves();

		m_physicsManager2D->RemoveReserved();
		m_physicsManager2D->GetCollideManager()->ReflectReserves();

		m_colEvent->ResetEvent();
		m_cameraManager->RemoveReserved();
	}
}	// namespace RHEngine
