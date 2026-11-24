/**
 * @file    GraphicsManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/19
 */

#include "pch.h"
#include "System/Render/GraphicsManager.h"

namespace RHEngine
{
	void GraphicsManager::Initialize(HWND hwnd, UINT width, UINT height)
	{
		// デバイスリソースの生成
		m_deviceResources = std::make_unique<DX::DeviceResources>();

		// デバイスの作成
		m_deviceResources->CreateDeviceResources();
		m_deviceResources->CreateWindowSizeDependentResources();

		// デバイス取得
		ID3D11Device* pDevice = m_deviceResources->GetD3DDevice();

		// デバイスリソースにウィンドウを渡す
		m_deviceResources->SetWindow(hwnd, width, height);

		// サンプラーリストの初期化
		m_samplerList.Initialize(pDevice);

		// 深度ステンシルステートの初期化
		m_depthStencilSateList.Initialize(pDevice);

		// ブレンドステートの初期化
		m_blendStateList.Initialize(pDevice);

		// ラスタライザステートの初期化
		m_rasterizerList.Initialize(pDevice);
	}
}
