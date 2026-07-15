//====================================================//
// ファイル名  : RendererManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/03
//
// 概要        :  描画コンポーネントを管理するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RendererManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	RendererManager::RendererManager()
		: m_addReserves{}
		, m_removeReserves{}
		, m_renderers{}
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
		for (auto& component : m_renderers)
		{
			// アクティブチェック
			if (!component->IsActive() || !component->GetOwn()->IsActive()) continue;

			component->Draw(renderer);
		}
	}
}	// namespace REngine
