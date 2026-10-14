//====================================================//
// ファイル名  : ShadowMap.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/Shadow/ShadowMap.h"

//====================================================//
// 関数の実体宣言
//====================================================//

bool REngine::ShadowMap::Initialize(ID3D11Device* device, UINT width, UINT height)
{
	// 作成不可なら失敗
	if (!device || width == 0 || height == 0) return false;

	// 各変数のリセット
	m_texture.Reset();
	m_srv.Reset();
	m_dsv.Reset();

	// ViewPortの設定
	m_viewport = {};
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	//---------- 深度テクスチャの作成 ----------//

	// Texture2D
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;		// 幅
	desc.Height = height;	// 高さ
	desc.MipLevels = 1;		// ミップマップのレベル数
	desc.ArraySize = 1;		// 扱うテクスチャの数
	desc.Format = DXGI_FORMAT_R32_TYPELESS;	// 色の表現方法
	desc.SampleDesc.Count = 1;		// マルチサンプリング設定
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	// 作成した構造体からテクスチャを作成
	HRESULT hr =
		device->CreateTexture2D(
			&desc, nullptr, m_texture.GetAddressOf()
		);

	// 失敗したら中断
	if (FAILED(hr)) return false;

	// ShaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;		// 型を明示
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(
		m_texture.Get(),
		&srvDesc,
		m_srv.GetAddressOf());

	if (FAILED(hr))
		return false;

	// DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = device->CreateDepthStencilView(
		m_texture.Get(),
		&dsvDesc,
		m_dsv.GetAddressOf());

	if (FAILED(hr))
		return false;

	return true;
}

void REngine::ShadowMap::Begin(ID3D11DeviceContext* context)
{
	if (!context) return;

	// 変更前の状態を保存
	context->OMGetRenderTargets(
		1,
		m_prevRTV.ReleaseAndGetAddressOf(),		// RenderTargetView
		m_prevDSV.ReleaseAndGetAddressOf());	// DepthStencilView

	UINT count = 1;
		context->RSGetViewports(
			&count,
			&m_prevViewport);	// ViewPort

	// 描画先を指定 
	// ShadowMapのため深度のみ書き込み先を指定
	context->OMSetRenderTargets(0, nullptr, m_dsv.Get());

	// ビューポートの指定
	context->RSSetViewports(1, &m_viewport);

	// 深度情報をクリア
	context->ClearDepthStencilView(
		m_dsv.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0);
}

void REngine::ShadowMap::End(ID3D11DeviceContext* context)
{
	if (!context) return;

	// 元の設定に戻す

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

ID3D11ShaderResourceView* REngine::ShadowMap::GetSRV() const
{
	return m_srv.Get();
}
