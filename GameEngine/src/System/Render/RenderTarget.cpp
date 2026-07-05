//====================================================//
// ファイル名  : RenderTarget.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/04
//
// 概要       : レンダーターゲット
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/Render/RenderTarget.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void RenderTarget::Create(ID3D11Device* device, unsigned int width, unsigned int height, bool useDepth, DXGI_FORMAT colorFormat, DXGI_FORMAT depthFormat)
{
	// 各変数のリセット
	m_texture.Reset();
	m_rtv.Reset();
	m_srv.Reset();
	m_depthTexture.Reset();
	m_dsv.Reset();

	// サイズを保存
	m_width = width;
	m_height = height;

	// ViewPortの設定
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(width);
	m_viewPort.Height = static_cast<float>(height);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	//---------- ColorTextureの作成 ----------//

	// Texture2D
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;		// 幅
	desc.Height = height;	// 高さ
	desc.MipLevels = 1;		// ミップマップのレベル数
	desc.ArraySize = 1;		// 扱うテクスチャの数
	desc.Format = colorFormat;	// 色の表現方法
	desc.SampleDesc.Count = 1;		// マルチサンプリング設定
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// 作成した構造体からテクスチャを作成
	device->CreateTexture2D(&desc, nullptr, m_texture.GetAddressOf());

	// RenderTargetView
	device->CreateRenderTargetView(
		m_texture.Get(),
		nullptr,
		m_rtv.GetAddressOf()
	);
	// ShaderResourceView
	device->CreateShaderResourceView(
		m_texture.Get(),
		nullptr,
		m_srv.GetAddressOf()
	);

	//---------- DepthTextureの作成 ----------//

	if (useDepth)
	{
		// Texture2D
		D3D11_TEXTURE2D_DESC depthDesc = {};

		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = depthFormat;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// デプスステンシルとして設定

		// 作成した構造体からTexture2Dを作成
		device->CreateTexture2D(
			&depthDesc,
			nullptr,
			m_depthTexture.GetAddressOf()
		);

		// DepthStencilView
		device->CreateDepthStencilView(
			m_depthTexture.Get(),
			nullptr,
			m_dsv.GetAddressOf()
		);
	}
}

void RenderTarget::Begin(ID3D11DeviceContext* context)
{
	// ShaderResourceを解除
	ID3D11ShaderResourceView* nullSRV[16] = {};
	context->PSSetShaderResources(0, 16, nullSRV);

	// 変更前の状態を保存
	context->OMGetRenderTargets(
		1,
		m_prevRTV.ReleaseAndGetAddressOf(),		// RenderTargetView
		m_prevDSV.ReleaseAndGetAddressOf());	// DepthStencilView

	UINT count = 1;
	context->RSGetViewports(
		&count,
		&m_prevViewport);	// ViewPort

	//--------- 設定 ----------//

	// RenderTarget
	context->OMSetRenderTargets(
		1,
		m_rtv.GetAddressOf(),
		m_dsv.Get());

	// ViewPort
	context->RSSetViewports(
		1,
		&m_viewPort);
}

void RenderTarget::End(ID3D11DeviceContext * context)
{
	// 変更前の状態へ戻す
	
	// RenderTarget
	context->OMSetRenderTargets(
		1,
		m_prevRTV.GetAddressOf(),
		m_prevDSV.Get());

	// ViewPort
	context->RSSetViewports(
		1,
		&m_prevViewport);
}

void RenderTarget::Clear(ID3D11DeviceContext* context, const DirectX::SimpleMath::Color& color)
{
	// RenderTargetViewのクリア
	context->ClearRenderTargetView(
		m_rtv.Get(),
		color);

	// DepthStencilViewのクリア
	if (m_dsv)
	{
		context->ClearDepthStencilView(
			m_dsv.Get(),
			D3D11_CLEAR_DEPTH,
			1.0f,
			0);
	}
}
