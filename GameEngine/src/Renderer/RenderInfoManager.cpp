/**
 * @file    RenderInfoManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/15
 */

#include "pch.h"
#include "Renderer/RenderInfoManager.h"

#include "Renderer/CBufferSlot.h"

namespace RHEngine
{
	void RenderInfoManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		// デバイスがなければ初期化不可
		if (!pDevice) return;

		// 作成結果を保持する変数
		HRESULT hr{};

		// PerFrame定数バッファの作成
		D3D11_BUFFER_DESC desc0 = {};
		desc0.ByteWidth = sizeof(PerFrame);
		desc0.Usage = D3D11_USAGE_DYNAMIC;
		desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc0.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = pDevice->CreateBuffer(&desc0, nullptr, m_perFrameCBuffer.ReleaseAndGetAddressOf());

		// 失敗したら中断
		if (FAILED(hr)) return;

		// PerCamera定数バッファの作成
		D3D11_BUFFER_DESC desc1 = {};
		desc1.ByteWidth = sizeof(PerCamera);
		desc1.Usage = D3D11_USAGE_DYNAMIC;
		desc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = pDevice->CreateBuffer(&desc1, nullptr, m_perCameraCBuffer.ReleaseAndGetAddressOf());

		if (FAILED(hr)) return;

		// World定数バッファの作成
		D3D11_BUFFER_DESC desc2 = {};
		desc2.ByteWidth = sizeof(PerObject);
		desc2.Usage = D3D11_USAGE_DYNAMIC;
		desc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = pDevice->CreateBuffer(&desc2, nullptr, m_perObjectCBuffer.ReleaseAndGetAddressOf());

		if (FAILED(hr)) return;

		// 全て成功すればコンテキストを保存
		m_pContext = pContext;
	}

	void RenderInfoManager::UpdatePerFrame(const SceneData& data)
	{
		// ライト情報の設定
		m_perFrame.light.pointLightCount = std::min(MAX_POINT_LIGHT, static_cast<int>(data.lightData.pointLights.size()));
		m_perFrame.light.spotLightCount = std::min(MAX_SPOT_LIGHT, static_cast<int>(data.lightData.spotLights.size()));
		m_perFrame.light.directionalLightCount = std::min(MAX_DIRECTIONAL_LIGHT, static_cast<int>(data.lightData.directionalLights.size()));

		for (int i = 0; i < m_perFrame.light.pointLightCount; ++i)
		{
			// ポイントライト情報の設定
			m_perFrame.light.pointLights[i] = data.lightData.pointLights[i];
		}

		for (int i = 0; i < m_perFrame.light.spotLightCount; ++i)
		{
			// スポットライト情報の設定
			m_perFrame.light.spotLights[i] = data.lightData.spotLights[i];
		}

		for (int i = 0; i < m_perFrame.light.directionalLightCount; ++i)
		{
			// ディレクショナルライト情報の設定
			m_perFrame.light.directionalLights[i] = data.lightData.directionalLights[i];
		}

		// 時間情報の設定
		m_perFrame.time.time = data.timeData.time;
		m_perFrame.time.elapsedTime = data.timeData.elapsedTime;

		// 影情報の設定
		m_perFrame.shadow.lightVP =
			(data.shadowData.lightCameraData.view * data.shadowData.lightCameraData.proj).Transpose();
		m_perFrame.shadow.shadowBias = data.shadowData.shadowBias;
	}

	void RenderInfoManager::UpdatePerCamera(const SceneCameraData& data)
	{
		// VP行列を取得
		DirectX::SimpleMath::Matrix view = data.view;
		DirectX::SimpleMath::Matrix proj = data.proj;
		DirectX::SimpleMath::Matrix vp = view * proj;

		// GPU用に転置して作成
		m_perCamera.view = view.Transpose();
		m_perCamera.proj = proj.Transpose();
		m_perCamera.vpMat = vp.Transpose();
		m_perCamera.invVpMat = vp.Invert().Transpose();
		m_perCamera.cameraPos = data.pos;
	}

	void RenderInfoManager::UpdatePerObject(const DirectX::SimpleMath::Matrix& world)
	{
		// World行列を取得
		m_perObject.world = world.Transpose();
		m_perObject.wvp = m_perCamera.vpMat * m_perObject.world;
		m_perObject.worldInvTranspose = world.Invert();
	}

	void RenderInfoManager::BindPerFrame()
	{
		// コンテキストがなければバインド不可
		if (!m_pContext) return;

		// マップを使用しCPU側から変更
		D3D11_MAPPED_SUBRESOURCE mapped;
		if (SUCCEEDED(m_pContext->Map(m_perFrameCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			std::memcpy(mapped.pData, &m_perFrame, sizeof(PerFrame));
			m_pContext->Unmap(m_perFrameCBuffer.Get(), 0);
		}

		// VP用スロットにバインド
		m_pContext->VSSetConstantBuffers(CBufferSlot::PerFrame, 1, m_perFrameCBuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(CBufferSlot::PerFrame, 1, m_perFrameCBuffer.GetAddressOf());
	}

	void RenderInfoManager::BindPerCamera()
	{
		// コンテキストがなければバインド不可
		if (!m_pContext) return;

		// マップを使用しCPU側から変更
		D3D11_MAPPED_SUBRESOURCE mapped;
		if (SUCCEEDED(m_pContext->Map(m_perCameraCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			std::memcpy(mapped.pData, &m_perCamera, sizeof(PerCamera));
			m_pContext->Unmap(m_perCameraCBuffer.Get(), 0);
		}

		// VP用スロットにバインド
		m_pContext->VSSetConstantBuffers(CBufferSlot::PerCamera, 1, m_perCameraCBuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(CBufferSlot::PerCamera, 1, m_perCameraCBuffer.GetAddressOf());
	}

	void RenderInfoManager::BindPerObject()
	{
		// コンテキストがなければバインド不可
		if (!m_pContext) return;

		// マップを使用しCPU側から変更
		D3D11_MAPPED_SUBRESOURCE mapped;
		if (SUCCEEDED(m_pContext->Map(m_perObjectCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			std::memcpy(mapped.pData, &m_perObject, sizeof(PerObject));
			m_pContext->Unmap(m_perObjectCBuffer.Get(), 0);
		}

		// VP用スロットにバインド
		m_pContext->VSSetConstantBuffers(CBufferSlot::PerObject, 1, m_perObjectCBuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(CBufferSlot::PerObject, 1, m_perObjectCBuffer.GetAddressOf());
	}
}	// namespace RHEngine	
