/**
 * @file    SkyboxComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/03
 */

#include "pch.h"
#include "Components/World/Renderer/Skybox/SkyboxComponent.h"

#include "System/Render/GraphicsManager.h"
#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"
#include "SkyboxEffect.h"

namespace RHEngine
{
	SkyboxComponent::SkyboxComponent(IComponentOwner* pOwner)
		: RendererBase(pOwner)
		, m_sky{}
		, m_effect{}
		, m_skyInputLayout{ nullptr }
		, m_pAssetManager{ nullptr }
	{
		ADD_PROPERTY(SkyboxComponent, m_textureHandle);

		// デバイスを取得
		auto* pDevice = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// コンテキストを取得
		auto pContext = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		// 球体を作成
		m_sky = DirectX::GeometricPrimitive::CreateGeoSphere(pContext, 2.f, 3, false);

		// スカイボックスエフェクトの生成
		m_effect = std::make_unique<SkyboxEffect>(pDevice);

		// インプットレイアウトの作成
		m_sky->CreateInputLayout(m_effect.get(),
			m_skyInputLayout.ReleaseAndGetAddressOf());
	}

	void SkyboxComponent::Draw(Renderer& renderer)
	{
		if (m_effect)
		{
			// プロジェクション行列を設定
			m_effect->SetProjection(renderer.GetProjection());
			// ビュー行列を設定
			m_effect->SetView(renderer.GetView());

			// 描画
			m_sky->Draw(m_effect.get(), m_skyInputLayout.Get());
		}
		// 設定されておらずHandleがあるなら取得を試みる
		if (!m_effect->HaveTexture() && m_textureHandle != ERROR_HANDLE<Texture>)
		{
			SetTexture(m_textureHandle);
		}
	}

	// 読み込み
	void SkyboxComponent::SetTexture(Handle<Texture> handle)
	{
		if (m_pAssetManager)
		{
			m_textureHandle = handle;

			// Textureを取得
			Texture* pTexture = m_pAssetManager->Get<Texture>(handle);

			// 取得できれば
			if (pTexture)
			{
				m_effect->SetTexture(pTexture->Get());
			}
		}
	}
}	// namespace RHEngine
