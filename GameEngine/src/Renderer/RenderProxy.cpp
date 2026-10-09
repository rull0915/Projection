//====================================================//
// ファイル名  : RenderProxy.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/RenderProxy.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void RenderProxy::Initialize(GraphicsSystem& system, DrawCommandContainer& container)
	{
		// 各レンダーの作成
		m_primitiveRenderer = std::make_unique<DrawPrimitiveCommandPublisher>(container);
		m_spriteRenderer = std::make_unique<DrawSpriteCommandPublisher>(container, system);
		m_meshRenderer = std::make_unique<DrawMeshCommandPublisher>(container, system);
		m_textRenderer = std::make_unique<DrawTextCommandPublisher>(container, system);
		m_uiRenderer = std::make_unique<DrawUICommandPublisher>(container);
	}
}
