//====================================================//
// ファイル名  : MainScreen.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/MainScreen.h"

#include "Renderer/Renderer.h"
#include "Input/MouseInput.h"

#include "System/ResourceManager.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

MainScreen::MainScreen()
	: m_defaultRenderTarget{ std::make_unique<RenderTarget>() }
	, m_draw{ true }
	, m_startPoint{ 0, 0 }
	, m_scale{ 1, 1 }
{
	// レンダーターゲットの初期化
	m_defaultRenderTarget->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		WindowManager::Instance().GetWidth(),
		WindowManager::Instance().GetHeight()
	);
}

void MainScreen::Render(Renderer& renderer)
{
	if (m_draw)
	{
		// 描画
		renderer.Draw().Sprite()
			.Extend(m_scale)
			.Execute(m_defaultRenderTarget->GetShaderResourceView(), m_startPoint);
	}
}

// メインスクリーンに補正したマウス座標を返す関数
DirectX::SimpleMath::Vector2 MainScreen::GetMousePointOnMainScreen()
{
	return (Input::Mouse::GetScaledMousePoint() - m_startPoint) / m_scale;
}
