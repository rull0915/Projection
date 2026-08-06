//====================================================//
// ファイル名  : DrawCommandExecutor.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/Command/DrawCommandExecutor.h"

#include "System/GraphicsManager.h"

#include "Assets/Types/MaterialAsset.h"
#include "Assets/Managers/AssetManager.h"

#include "Renderer/CBufferSlot.h"

//====================================================//
// 関数の実体宣言
//====================================================//

REngine::DrawCommandExecutor::DrawCommandExecutor(AssetManager& assetManager)
	: m_primitiveBatch{}
	, m_basicEffect{}
	, m_spriteBatch{}
	, m_inputLayout{}
	, m_view{}
	, m_projection{}
	, m_pDevice{ nullptr }
	, m_pContext{ nullptr }
	, m_pStates{ nullptr }
	, m_assetManager{ assetManager }
{}

void REngine::DrawCommandExecutor::Initialize()
{
	// デバイスリソースの取得
	DX::DeviceResources* dr = GraphicsManager::Instance().GetDeviceResources();

	m_pDevice = dr->GetD3DDevice();

	m_pContext = dr->GetD3DDeviceContext();

	m_pStates = dr->GetCommonStates();

	// プリミティブバッチの初期化
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_pContext);

	// スプライトバッチの初期化
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pContext);

	// ベーシックエフェクトの初期化
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_pDevice);

	// 設定
	m_basicEffect->SetVertexColorEnabled(true);	// 頂点カラーの使用をオン
	m_basicEffect->SetLightingEnabled(false);	// ライトの使用をオフ
	m_basicEffect->SetTextureEnabled(false);	// テクスチャの使用をオフ

	// インプットレイアウトの作成
	// 例外処理
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			m_pDevice,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()));

	// WVP行列の初期化
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);			// ワールド
	m_basicEffect->SetView(DirectX::SimpleMath::Matrix::Identity);			// ビュー
	m_basicEffect->SetProjection(DirectX::SimpleMath::Matrix::Identity);	// プロジェクション

	// VP定数バッファの作成
	D3D11_BUFFER_DESC desc0 = {};
	desc0.ByteWidth = sizeof(VPBuffer);
	desc0.Usage = D3D11_USAGE_DYNAMIC;
	desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc0.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_pDevice->CreateBuffer(&desc0, nullptr, m_vpConstantBuffer.ReleaseAndGetAddressOf());

	// World定数バッファの作成
	D3D11_BUFFER_DESC desc1 = {};
	desc1.ByteWidth = sizeof(WorldBuffer);
	desc1.Usage = D3D11_USAGE_DYNAMIC;
	desc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_pDevice->CreateBuffer(&desc1, nullptr, m_worldConstantBuffer.ReleaseAndGetAddressOf());
}

void REngine::DrawCommandExecutor::DrawCommandExecute(DrawCommandContainer& container, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// ビュー行列の取得
	m_view = view;

	// プロジェクション行列の取得
	m_projection = proj;

	// 各コマンドの実行
	DrawPrimitiveCommandExecute(container.GetDrawPrimitiveCommands());
	DrawModelCommandExecute(container.GetDrawModelCommands());
	DrawSpriteAndFontCommandExecute(container.GetDrawSpriteCommands(), container.GetDrawTextCommands());
}

void REngine::DrawCommandExecutor::DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands)
{
	// コマンド分ループ
	for (auto& c : commands)
	{
		// マテリアルを調べる
		MaterialAsset* material = m_assetManager.Get<MaterialAsset>(c.material);

		// 描画準備
		PreparePrimitiveRendering();

		// 存在しており有効ハンドルなら
		if (material && material->IsValid())
		{
			// バインド
			material->Bind(m_pContext, m_assetManager);
		
			// VP行列をバインド
			BindVPBuffer();

			// ワールド行列をバインド
			BindWorldBuffer(c.world);

			// 定数バッファを更新
			material->UpdateConstantBuffers(m_pDevice, m_pContext, m_assetManager);
		}
		// なければ
		else
		{
			// ワールド行列のセット
			m_basicEffect->SetWorld(c.world);

			// View
			m_basicEffect->SetView(m_view);

			// Projection
			m_basicEffect->SetProjection(m_projection);

			// インプットレイアウトをセット
			m_pContext->IASetInputLayout(m_inputLayout.Get());

			// エフェクトを適用
			m_basicEffect->Apply(m_pContext);
		}

		// 描画開始
		m_primitiveBatch->Begin();

		// 線
		if (!c.lines.empty()) m_primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, &c.lines.begin()->s, c.lines.size() * 2);

		// 三角形
		if (!c.triangles.empty()) m_primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &c.triangles.begin()->a, c.triangles.size() * 3);

		// 描画終了
		m_primitiveBatch->End();
	}
}

void REngine::DrawCommandExecutor::DrawSpriteAndFontCommandExecute(const std::vector<DrawSpriteCommand>& spriteCommands, const std::vector<DrawTextCommand>& textCommands)
{
	// スプライトバッチの開始
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_pStates->NonPremultiplied());

	// 全コマンドの描画
	for (auto& c : spriteCommands)
	{
		// テクスチャがあれば描画
		if (c.pTexture) 
			m_spriteBatch->Draw(c.pTexture, c.pos, (c.srcRect ? &c.srcRect.value() : nullptr), c.color, c.angle, c.origin, c.scale);
	}
	for (auto& c : textCommands)
	{
		// フォントがあれば描画
		if (c.spriteFont)
			c.spriteFont->DrawString(m_spriteBatch.get(), c.text.c_str(), c.pos, c.color, c.angle, c.origin, c.scale);
	}

	// 終了
	m_spriteBatch->End();
}

void REngine::DrawCommandExecutor::DrawModelCommandExecute(const std::vector<DrawModelCommand>& commands)
{
	// 全コマンドを描画
	for (auto& c : commands)
	{
		// 描画
		c.pModel->Draw(m_pContext, *m_pStates, c.world, m_view, m_projection);
	}
}

void REngine::DrawCommandExecutor::PreparePrimitiveRendering()
{
	// 透明度を使用
	m_pContext->OMSetBlendState(m_pStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// デプスソートを使用
	m_pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);

	// カリング不使用
	m_pContext->RSSetState(m_pStates->CullNone());

	// サンプラーをリセット
	ID3D11SamplerState* samplers[] = { nullptr };
	m_pContext->PSSetSamplers(0, 1, samplers);
}

void REngine::DrawCommandExecutor::BindVPBuffer()
{
	// 定数バッファを用意
	VPBuffer vpData;

	// 行列を掛けて渡す (転置が必要かも)
	vpData.view = m_view.Transpose();
	vpData.proj = m_projection.Transpose();

	// マップを使用しCPU側から変更
	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(m_pContext->Map(m_vpConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		std::memcpy(mapped.pData, &vpData, sizeof(VPBuffer));
		m_pContext->Unmap(m_vpConstantBuffer.Get(), 0);
	}

	// VP用スロットにバインド
	m_pContext->VSSetConstantBuffers(CBufferSlot::PerFrame, 1, m_vpConstantBuffer.GetAddressOf());
}

void REngine::DrawCommandExecutor::BindWorldBuffer(const DirectX::SimpleMath::Matrix& world)
{
	// 定数バッファを用意
	WorldBuffer worldData;

	// 行列を掛けて渡す (転置が必要かも)
	worldData.world = world.Transpose();

	// マップを使用しCPU側から変更
	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(m_pContext->Map(m_worldConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		std::memcpy(mapped.pData, &worldData, sizeof(WorldBuffer));
		m_pContext->Unmap(m_worldConstantBuffer.Get(), 0);
	}

	// World用スロットにバインド
	m_pContext->VSSetConstantBuffers(CBufferSlot::PerObject, 1, m_worldConstantBuffer.GetAddressOf());
}
