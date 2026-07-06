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

#include "System/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "SkyboxEffect.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SkyboxComponent::SkyboxComponent(IComponentOwner* own)
	: RendererBase(own)
	, m_sky{}
	, m_effect{}
	, m_skyInputLayout{ nullptr }
	, m_keyName{}
	, m_texture{ nullptr }
{
	ADD_PROPERTY(m_keyName);
}

// 生成直後に一度呼ばれます
void SkyboxComponent::Awake()
{
	// デバイスを取得
	auto* device = ResourceManager::Instance().GetResources()->GetD3DDevice();

	// コンテキストを取得
	auto context = ResourceManager::Instance().GetResources()->GetD3DDeviceContext();

	// 球体を作成
	m_sky = DirectX::GeometricPrimitive::CreateGeoSphere(context, 2.f, 3,
		false /*invert for being inside the shape*/);

	// スカイボックスエフェクトの生成
	m_effect = std::make_unique<SkyboxEffect>(device);

	// インプットレイアウトの作成
	m_sky->CreateInputLayout(m_effect.get(),
		m_skyInputLayout.ReleaseAndGetAddressOf());
}

void SkyboxComponent::Start()
{
	LoadResource();

	ReflectLoading();
}

void SkyboxComponent::Draw(Renderer& renderer)
{
	if (m_effect)
	{
		// プロジェクション行列を設定
		m_effect->SetProjection(renderer.GetRenderState().GetProjection());
		// ビュー行列を設定
		m_effect->SetView(renderer.GetRenderState().GetView());

		// 描画
		m_sky->Draw(m_effect.get(), m_skyInputLayout.Get());
	}
}

// 読み込み
void SkyboxComponent::SetTexture(const std::string& key)
{
	m_keyName = key;

	// テクスチャの読み込み
	if (m_effect)
	{
		LoadResource();

		ReflectLoading();
	}
}

void SkyboxComponent::ReflectLoading()
{
	if (m_texture && m_effect) m_effect->SetTexture(m_texture);
}
