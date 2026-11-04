/**
 * @file    ShadowRenderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 */

#include "pch.h"
#include "Renderer/Shadow/ShadowRenderer.h"

#include "Assets/Types/Vertex/VertexTypes.h"
#include "Assets/EngineDefaultAssets.h"
#include "Assets/Managers/AssetManager.h"
#include "System/Render/GraphicsManager.h"

namespace RHEngine
{
	ShadowRenderer::ShadowRenderer(AssetManager& assetManager)
		: m_usable{ false }
		, m_drawMeshCommands{}
		, m_vsShaderHandle{}
		, m_assetManager{ assetManager }
		, m_pDevice{ nullptr }
		, m_pContext{ nullptr }
		, m_pStates{ nullptr }
		, m_currentShadowMap{ nullptr }
		, m_depthStencilState{}
		, m_infoManager{}
	{}

	void ShadowRenderer::Initialize()
	{
		// ゲームエンジンデフォルトのShadowPass用vsを取得
		m_vsShaderHandle = m_assetManager.LoadFromUUID(DefaultAsset::SHADOW_PASS_VS).As<ShaderAsset>();

		// デバイスリソースの取得
		DX::DeviceResources* dr = GraphicsManager::Instance().GetDeviceResources();

		m_pDevice = dr->GetD3DDevice();

		m_pContext = dr->GetD3DDeviceContext();

		m_pStates = dr->GetCommonStates();

		// 情報マネージャーを初期化
		m_infoManager.Initialize(m_pDevice, m_pContext);

		// DepthStencilStateの作成
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = FALSE;
		HRESULT hr = m_pDevice->CreateDepthStencilState(&desc, m_depthStencilState.GetAddressOf());

		if (FAILED(hr)) return;

		// 全て成功したら使用可能に
		m_usable = true;
	}

	void ShadowRenderer::Begin(ShadowMap* pShadowMap)
	{
		// 使用不可ならスキップ
		if (!m_usable) return;

		// 既に描画中かShadowMapが指定されていないならスキップ
		if (m_currentShadowMap || !pShadowMap) return;

		// 指定されたシャドウマップへの描画を開始
		pShadowMap->Begin(m_pContext);

		// 使用中のシャドウマップとして保存
		m_currentShadowMap = pShadowMap;
	}

	void ShadowRenderer::End()
	{
		if (!m_currentShadowMap || !m_usable) return;

		// シャドウマップへの描画を終了
		m_currentShadowMap->End(m_pContext);

		// 使用を解除
		m_currentShadowMap = nullptr;
	}

	void ShadowRenderer::ExecuteCommands(const SceneCameraData& data)
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
		ShaderAsset* pVs = m_assetManager.Get(m_vsShaderHandle);

		// 取得できなければ描画不可
		if (!pVs)
		{
			// 描画コマンドのクリア
			m_drawMeshCommands.clear();

			return;
		}

		// InputLayoutを取得
		auto inputLayout = GraphicsManager::Instance().GetInputLayoutCache().GetOrCreate(
			m_pDevice, static_cast<uint32_t>(typeid(VertexPositionNormalTexture).hash_code()),
			pVs, VertexPositionNormalTexture::GetLayout()
		);

		// 入力レイアウトの設定 
		m_pContext->IASetInputLayout(inputLayout.Get());

		// トポロジーの設定 
		m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 頂点シェーダの設定
		pVs->Bind(m_pContext);

		// ピクセルシェーダーを明示的にNULLにする
		m_pContext->PSSetShader(nullptr, nullptr, 0);

		// カメラの設定 (ライトのVP)
		m_infoManager.UpdatePerCamera(data);
		m_infoManager.BindPerCamera();

		// カリング
		m_pContext->RSSetState(m_pStates->CullClockwise());

		// デプスステンシルステートを設定
		m_pContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

		for (auto& command : m_drawMeshCommands)
		{
			// 描画するメッシュを取得
			Mesh* pMesh = command.pMesh;

			// なければ次へ
			if (!pMesh) continue;

			// 頂点バッファの作成
			ID3D11Buffer* vsBuffers[] = { pMesh->GetVertexBuffer() };
			UINT strides[] = { sizeof(VertexPositionNormalTexture) };
			UINT offsets[] = { 0 };

			// ワールド行列のセット
			m_infoManager.UpdatePerObject(command.world);
			m_infoManager.BindPerObject();

			// 頂点バッファの設定
			m_pContext->IASetVertexBuffers(0, 1, vsBuffers, strides, offsets);

			// インデックスバッファの設定
			m_pContext->IASetIndexBuffer(pMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// 範囲外サブメッシュの場合次へ
			if (command.subMeshIndex >= pMesh->GetSubMeshes().size()) continue;

			// サブメッシュの取得
			auto& subMesh = pMesh->GetSubMeshes()[command.subMeshIndex];

			// 描画命令
			m_pContext->DrawIndexed(subMesh.indexCount, subMesh.indexOffset, subMesh.vertexOffset);
		}

		// 描画コマンドのクリア
		m_drawMeshCommands.clear();
	}
}	// namespace RHEngine
