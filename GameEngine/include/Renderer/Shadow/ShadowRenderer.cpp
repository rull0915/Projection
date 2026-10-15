//====================================================//
// ファイル名  : ShadowRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ShadowRenderer.h"
#include "Assets/Types/Vertex/VertexTypes.h"

#include "Assets/EngineDefaultAssets.h"
#include "Assets/Managers/AssetManager.h"
#include "Assets/Types/Shader/InputLayoutCache.h"
#include "System/GraphicsManager.h"

#include "Renderer/CBufferSlot.h"

//====================================================//
// 関数の実体宣言
//====================================================//

REngine::ShadowRenderer::ShadowRenderer(AssetManager& assetManager)
	: m_usable{ false }
	, m_drawMeshCommands{}
	, m_vsShaderHandle{}
	, m_assetManager{ assetManager }
	, m_pDevice{ nullptr }
	, m_pContext{ nullptr }
	, m_pStates{ nullptr }
	, m_currentShadowMap{ nullptr }
	, m_perFrameConstantBuffer{}
	, m_perObjectConstantBuffer{}
	, m_depthStencilState{}
{
}

void REngine::ShadowRenderer::Initialize()
{
	// ゲームエンジンデフォルトのShadowPass用vsを取得
	m_vsShaderHandle = m_assetManager.LoadFromUUID(DefaultAsset::SHADOW_PASS_VS).As<ShaderAsset>();

	// デバイスリソースの取得
	DX::DeviceResources* dr = GraphicsManager::Instance().GetDeviceResources();

	m_pDevice = dr->GetD3DDevice();

	m_pContext = dr->GetD3DDeviceContext();

	m_pStates = dr->GetCommonStates();

	// PerFrame定数バッファの作成
	D3D11_BUFFER_DESC desc0 = {};
	desc0.ByteWidth = sizeof(ShadowPassPerFrameCBuffer);
	desc0.Usage = D3D11_USAGE_DYNAMIC;
	desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc0.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = m_pDevice->CreateBuffer(&desc0, nullptr, m_perFrameConstantBuffer.ReleaseAndGetAddressOf());

	// 失敗したら使用不可のまま中断
	if (FAILED(hr)) return;

	// PerObject定数バッファの作成
	D3D11_BUFFER_DESC desc1 = {};
	desc1.ByteWidth = sizeof(ShadowPassPerObjectCBuffer);
	desc1.Usage = D3D11_USAGE_DYNAMIC;
	desc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = m_pDevice->CreateBuffer(&desc1, nullptr, m_perObjectConstantBuffer.ReleaseAndGetAddressOf());

	if (FAILED(hr)) return;

	// DepthStencilStateの作成
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = TRUE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = FALSE;
	hr = m_pDevice->CreateDepthStencilState(&desc, m_depthStencilState.GetAddressOf());

	if (FAILED(hr)) return;

	// 全て成功したら使用可能に
	m_usable = true;
}

void REngine::ShadowRenderer::Begin(ShadowMap* shadowMap)
{
	// 使用不可ならスキップ
	if (!m_usable) return;

	// 既に描画中かShadowMapが指定されていないならスキップ
	if (m_currentShadowMap || !shadowMap) return;

	// 指定されたシャドウマップへの描画を開始
	shadowMap->Begin(m_pContext);

	// 使用中のシャドウマップとして保存
	m_currentShadowMap = shadowMap;
}

void REngine::ShadowRenderer::End()
{
	if (!m_currentShadowMap || !m_usable) return;

	// シャドウマップへの描画を終了
	m_currentShadowMap->End(m_pContext);

	// 使用を解除
	m_currentShadowMap = nullptr;
}

void REngine::ShadowRenderer::ExecuteCommands(const DirectX::SimpleMath::Matrix& lightView, const DirectX::SimpleMath::Matrix& lightProj)
{
	// 使用不可ならスキップ
	if (!m_usable) return;

	// シャドウマップを使用していない場合スキップ
	if (!m_currentShadowMap)
	{
		// 描画コマンドのクリア
		m_drawMeshCommands.clear();

		return;
	}

	// 各ステージの設定

	// VSの取得
	ShaderAsset* vs = m_assetManager.Get(m_vsShaderHandle);

	// 取得できなければ描画不可
	if (!vs)
	{
		// 描画コマンドのクリア
		m_drawMeshCommands.clear();

		return;
	}

	// InputLayoutを取得
	auto inputLayout = InputLayoutCache::Instance().GetOrCreate(
		m_pDevice, static_cast<uint32_t>(typeid(VertexPositionNormalTexture).hash_code()),
		vs->GetBlob(), VertexPositionNormalTexture::GetLayout()
	);

	// 入力レイアウトの設定 
	m_pContext->IASetInputLayout(inputLayout.Get());

	// トポロジーの設定 
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点シェーダの設定
	vs->Bind(m_pContext);

	// VP行列の設定
	ShadowPassPerFrameCBuffer perFrame{ (lightView * lightProj).Transpose() };

	BindPerFrame(perFrame);

	// カリング
	m_pContext->RSSetState(m_pStates->CullClockwise());

	// デプスステンシルステートを設定
	m_pContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	for (auto& command : m_drawMeshCommands)
	{
		// 描画するメッシュを取得
		Mesh* mesh = command.pMesh;

		// なければ次へ
		if (!mesh) continue;

		// 頂点バッファの作成
		ID3D11Buffer* vsBuffers[] = { mesh->GetVertexBuffer() };
		UINT strides[] = { sizeof(VertexPositionNormalTexture) };
		UINT offsets[] = { 0 };

		ShadowPassPerObjectCBuffer perObject{ command.world };

		// ワールド行列のセット
		BindPerObject(perObject);

		// 頂点バッファの設定
		m_pContext->IASetVertexBuffers(0, 1, vsBuffers, strides, offsets);

		// インデックスバッファの設定
		m_pContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// 範囲外サブメッシュの場合次へ
		if (command.subMeshIndex >= mesh->GetSubMeshes().size()) continue;

		// サブメッシュの取得
		auto& subMesh = mesh->GetSubMeshes()[command.subMeshIndex];

		// 描画命令
		m_pContext->DrawIndexed(subMesh.indexCount, subMesh.indexOffset, subMesh.vertexOffset);
	}

	// 描画コマンドのクリア
	m_drawMeshCommands.clear();
}

void REngine::ShadowRenderer::BindPerFrame(const ShadowPassPerFrameCBuffer& buf)
{
	// マップを使用しCPU側から変更
	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(m_pContext->Map(m_perFrameConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		std::memcpy(mapped.pData, &buf, sizeof(ShadowPassPerFrameCBuffer));
		m_pContext->Unmap(m_perFrameConstantBuffer.Get(), 0);
	}

	// PerFrame用スロットにバインド
	m_pContext->VSSetConstantBuffers(CBufferSlot::PerFrame, 1, m_perFrameConstantBuffer.GetAddressOf());
}

void REngine::ShadowRenderer::BindPerObject(const ShadowPassPerObjectCBuffer& buf)
{
	// マップを使用しCPU側から変更
	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(m_pContext->Map(m_perObjectConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		std::memcpy(mapped.pData, &buf, sizeof(ShadowPassPerObjectCBuffer));
		m_pContext->Unmap(m_perObjectConstantBuffer.Get(), 0);
	}

	// PerObject用スロットにバインド
	m_pContext->VSSetConstantBuffers(CBufferSlot::PerObject, 1, m_perObjectConstantBuffer.GetAddressOf());
}
