/**
 * @file    Renderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 */

#include "pch.h"
#include "Renderer/Renderer.h"

#include "System/Render/GraphicsManager.h"

namespace RHEngine
{
	Renderer::Renderer(AssetManager& am)
		: m_renderProxy{}
		, m_graphicSystem{ am, m_infoManager }
		, m_isInitialized{ false }
	{
	}

	void Renderer::Initialize()
	{
		// 初期化済みなら何もしない
		if (m_isInitialized) return;

		// デバイスリソースの取得
		auto* dr = GraphicsManager::Instance().GetDeviceResources();

		// 仲介クラスの初期化
		m_renderProxy.Initialize(m_graphicSystem, m_graphicSystem.GetCommandContainer());

		// 描画システムの初期化
		m_graphicSystem.Initialize();

		// 情報マネージャーの初期化
		m_infoManager.Initialize(dr->GetD3DDevice(), dr->GetD3DDeviceContext());

		// フラグのセット
		m_isInitialized = true;
	}

	void Renderer::Begin(const SceneData& data)
	{
		// PerFrameバッファを更新
		m_infoManager.UpdatePerFrame(data);

		// バインド
		m_infoManager.BindPerFrame();
	}

	void Renderer::ChangeCamera(const SceneCameraData& data)
	{
		// PerFrameバッファを更新
		m_infoManager.UpdatePerCamera(data);

		// バインド
		m_infoManager.BindPerCamera();

		// 保存
		m_currentCamera = data;
	}

	void Renderer::ExecuteCommands()
	{
		// ため込んでいた描画を行う
		m_graphicSystem.ExecuteCommands();
	}

	void Renderer::End()
	{
		// ため込んでいた描画を行う
		m_graphicSystem.ExecuteCommands();
	}
}	// namespace RHEngine
