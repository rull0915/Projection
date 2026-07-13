//====================================================//
// ファイル名  : SceneRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/SceneRenderer.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

#include "System/ResourceManager.h"
#include "Debug/DebugManager.h"

#include "Components/ComponentBase.h"
#include "Scene/UpdatePipeline.h"

//====================================================//
// 関数の実体宣言
//====================================================//

SceneRenderer::SceneRenderer(Scene* pScene)
	: m_pScene{ pScene }
{
}

void SceneRenderer::RenderWithContext(const RenderContext& context, Renderer& renderer)
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
		m_pScene->GetPipeline()->DrawWorld(renderer);
	}

	// 色 (緑)
	static const DirectX::SimpleMath::Color color = { 0, 1, 0, 1 };

	// Worldのデバッグ描画
	if (context.flags & DrawFlag::WorldDebug && DebugManager::Instance().IsDrawDebugOnWorld())
	{
		for (auto& c : m_pScene->GetComponentRegister()->GetDebugRenders())
		{
			// キャスト
			if (auto* com = dynamic_cast<ComponentBase*>(c))
			{
				if (!com->IsActive() || !com->GetOwn()->IsActive()) continue;

				c->DebugRender(renderer, color);
			}
		}
	}

	// 行列のリセット
	renderer.SetView(DirectX::SimpleMath::Matrix::Identity);
	renderer.SetProjection(DirectX::SimpleMath::Matrix::Identity);

	// UIの描画
	if (context.flags & DrawFlag::UI)
	{
		m_pScene->GetPipeline()->DrawUI(renderer);
	}

	// RectTransformのデバッグ描画
	if (context.flags & DrawFlag::UIDebug && DebugManager::Instance().IsDrawDebugOnUI())
	{
		m_pScene->GetPipeline()->DrawRects(renderer, color);
	}

	// Rendererの終了
	renderer.End();

	// 描画の終了
	context.target->End(deviceContext);
}
