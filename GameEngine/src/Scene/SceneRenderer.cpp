/**
 * @file    SceneRenderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/12
 */

#include "pch.h"
#include "Scene/SceneRenderer.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "Scene/ComponentRegister.h"

#include "System/Render/GraphicsManager.h"
#include "Debug/DebugManager.h"

#include "Components/ComponentBase.h"
#include "Scene/UpdatePipeline.h"
#include "Renderer/Shadow/ShadowRenderer.h"

#include "Components/World/Renderer/MeshRenderer.h"

namespace RHEngine
{
	SceneRenderer::SceneRenderer(Scene* pScene)
		: m_pScene{ pScene }
	{}

	void SceneRenderer::DrawShadowPass(const ShadowRenderContext& ctx, ShadowRenderer& renderer)
	{
		// シャドウマップが未設定なら描画しない
		if (!ctx.pShadowMap) return;

		// 描画開始
		renderer.Begin(ctx.pShadowMap);

		// 描画コマンドの発行
		for (auto& pComponent : m_pScene->GetComponentRegister()->GetAllComponents<MeshRenderer>())
		{
			// メッシュレンダラーに変換
			MeshRenderer* pMesh = static_cast<MeshRenderer*>(pComponent);

			// 描画条件を満たしていればなら
			if (pMesh->IsActive() && pMesh->GetOwn()->IsActive())
			{
				// シャドウマップへ描画
				pMesh->DrawToShadowMap(renderer);
			}
		}

		// 描画コマンドの実行
		renderer.ExecuteCommands(ctx.cameraData);

		// 描画終了
		renderer.End();
	}

	void SceneRenderer::DrawWorldPass(const RenderContext& ctx, Renderer& renderer)
	{
		auto* pDeviceContext = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		// 描画開始 未設定の場合直接バックバッファへ描画
		if (ctx.target)
		{
			ctx.target->Begin(pDeviceContext);

			// クリア
			if (ctx.clearTarget) ctx.target->Clear(pDeviceContext, ctx.backColor);
		}

		// カメラデータを設定
		renderer.ChangeCamera(ctx.cameraData);

		// ワールドデータのリセット
		renderer.SetWorld(DirectX::SimpleMath::Matrix::Identity);
		renderer.SetMaterial(ERROR_HANDLE<MaterialAsset>);

		// Worldの描画
		if (ctx.flags & DrawFlag::World)
		{
			// 描画管理クラスの描画処理
			m_pScene->GetPipeline()->DrawWorld(renderer);
		}

		// ワールドデータのリセット
		renderer.SetWorld(DirectX::SimpleMath::Matrix::Identity);
		renderer.SetMaterial(ERROR_HANDLE<MaterialAsset>);

		// Worldのデバッグ描画
		if (ctx.flags & DrawFlag::WorldDebug && DebugManager::Instance().IsDrawDebugOnWorld())
		{
			for (auto& pDebugRenderable : m_pScene->GetComponentRegister()->GetDebugRenders())
			{
				// キャスト
				if (auto* pComponent = dynamic_cast<ComponentBase*>(pDebugRenderable))
				{
					if (!pComponent->IsActive() || !pComponent->GetOwn()->IsActive()) continue;

					pDebugRenderable->DebugRender(renderer, DEBUG_COLOR);
				}
			}
		}

		// 描画コマンドの発行
		renderer.ExecuteCommands();

		// 描画終了
		if (ctx.target)
		{
			ctx.target->End(pDeviceContext);
		}
	}

	void SceneRenderer::DrawUIPass(const RenderContext& ctx, Renderer& renderer)
	{
		// カメラデータを設定
		renderer.ChangeCamera(ctx.cameraData);

		// UIの描画
		if (ctx.flags & DrawFlag::UI)
		{
			m_pScene->GetPipeline()->DrawUI(renderer);
		}

		// RectTransformのデバッグ描画
		if (ctx.flags & DrawFlag::UIDebug && DebugManager::Instance().IsDrawDebugOnUI())
		{
			m_pScene->GetPipeline()->DrawRects(renderer, DEBUG_COLOR);
		}

		auto* pDeviceContext = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		// 設定されていればターゲットへの描画を開始
		if (ctx.target)
		{
			ctx.target->Begin(pDeviceContext);

			// 上から描画する想定のため背景は完全透明
			if (ctx.clearTarget) ctx.target->Clear(pDeviceContext, ctx.backColor);
		}

		// 描画コマンドの発行
		renderer.ExecuteCommands();

		// 設定されていればターゲットへの描画を終了
		if (ctx.target)
		{
			ctx.target->End(pDeviceContext);
		}
	}
}	// namespace RHEngine
