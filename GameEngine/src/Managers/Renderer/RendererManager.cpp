/**
 * @file    RendererManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/03
 */

#include "pch.h"
#include "RendererManager.h"

namespace RHEngine
{
	RendererManager::RendererManager()
	{}

	RendererManager::~RendererManager()
	{}

	/// <summary>
	/// 全ての描画を行う関数
	/// </summary>
	void RendererManager::DrawAll(Renderer& renderer)
	{
		ReflectReserves();

		// 管理している全てのコンポーネントの描画処理を呼び出す
		for (auto& component : GetObjects())
		{
			// アクティブチェック
			if (!component->IsActive() || !component->GetOwn()->IsActive()) continue;

			component->Draw(renderer);
		}
	}
}	// namespace RHEngine
