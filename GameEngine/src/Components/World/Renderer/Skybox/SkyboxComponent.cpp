//====================================================//
// ファイル名  : SkyboxComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/03
//
// 概要       : スカイボックスコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Renderer/Skybox/SkyboxComponent.h"

#include "System/GraphicsManager.h"
#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"
#include "SkyboxEffect.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	SkyboxComponent::SkyboxComponent(IComponentOwner* own)
		: RendererBase(own)
		, m_sky{}
		, m_effect{}
		, m_skyInputLayout{ nullptr }
	{
		ADD_PROPERTY(SkyboxComponent, m_textureHandle);

		// デバイスを取得
		auto* device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// コンテキストを取得
		auto context = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		// 球体を作成
		m_sky = DirectX::GeometricPrimitive::CreateGeoSphere(context, 2.f, 3, false);

		// スカイボックスエフェクトの生成
		m_effect = std::make_unique<SkyboxEffect>(device);

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
		if (m_assetManager)
		{
			m_textureHandle = handle;

			// Textureを取得
			Texture* texture = m_assetManager->Get<Texture>(handle);

			// 取得できれば
			if (texture)
			{
				m_effect->SetTexture(texture->Get());
			}
		}
	}

}	// namespace REngine
