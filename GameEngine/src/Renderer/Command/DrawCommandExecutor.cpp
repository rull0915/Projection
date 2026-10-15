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

#include "System/Render/GraphicsManager.h"

#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/AssetManager.h"
#include "Assets/Types/Vertex/VertexTypes.h"

//====================================================//
// 関数の実体宣言
//====================================================//

REngine::DrawCommandExecutor::DrawCommandExecutor(AssetManager& assetManager, RenderInfoManager& infoManager)
	: m_primitiveBatch{}
	, m_spriteBatch{}
	, m_infoManager{ infoManager }
	, m_pDevice{ nullptr }
	, m_pContext{ nullptr }
	, m_pStates{ nullptr }
	, m_assetManager{ assetManager }
{}

REngine::DrawCommandExecutor::~DrawCommandExecutor() = default;

void REngine::DrawCommandExecutor::Initialize()
{
	// デバイスリソースの取得
	DX::DeviceResources* dr = GraphicsManager::Instance().GetDeviceResources();

	m_pDevice = dr->GetD3DDevice();

	m_pContext = dr->GetD3DDeviceContext();

	m_pStates = dr->GetCommonStates();

	// プリミティブバッチの初期化
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_pContext);

	// スプライトバッチの初期化
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pContext);
}

void REngine::DrawCommandExecutor::DrawCommandExecute(DrawCommandContainer& container)
{
	// 各コマンドの実行
	DrawModelCommandExecute(container.GetDrawModelCommands());
	DrawPrimitiveCommandExecute(container.GetDrawPrimitiveCommands());
	DrawSpriteAndFontCommandExecute(container.GetDrawUICommands());
}

void REngine::DrawCommandExecutor::DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands)
{
	// コマンド分ループ
	for (auto& c : commands)
	{
		// マテリアルを調べる
		MaterialAsset* material = m_assetManager.Get<MaterialAsset>(c.material);

		// 透明度を不使用
		m_pContext->OMSetBlendState(m_pStates->Opaque(), nullptr, 0xFFFFFFFF);

		// デプスソートを使用
		m_pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);

		// カリング不使用
		m_pContext->RSSetState(m_pStates->CullNone());

		// 存在しており有効ハンドルなら
		if (material && material->IsValid())
		{
			// バインド
			material->Apply(m_pContext);

			// InfoManager経由のPerObjectバッファ更新
			m_infoManager.UpdatePerObject(c.world);
			m_infoManager.BindPerObject();

			// 定数バッファを更新
			material->UpdateConstantBuffers(m_pDevice, m_pContext);
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

void REngine::DrawCommandExecutor::DrawSpriteAndFontCommandExecute(const std::vector<DrawCommandContainer::DrawUICommand>& commands)
{
	// スプライトバッチの開始
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_pStates->NonPremultiplied());

	// マテリアルハンドル
	Handle<MaterialAsset> oldHandle = { 0, 0 };

	// 全コマンドの描画
	for (auto& command : commands)
	{
		// マテリアルの取得
		Handle<MaterialAsset> matHandle = std::visit([this](auto& c) { return c.material; }, command);

		MaterialAsset* mat = m_assetManager.Get(matHandle);

		// マテリアルが変わっていた場合
		if (matHandle != oldHandle)
		{
			// マテリアル情報の更新
			oldHandle = matHandle;

			// 一度終了
			m_spriteBatch->End();

			// 再度Begin
			if (mat)
			{
				m_spriteBatch->Begin(
					DirectX::SpriteSortMode_Deferred,
					m_pStates->NonPremultiplied(),
					nullptr,
					nullptr,
					nullptr,
					[this, mat]()
					{
						mat->BindShader(m_pContext, ShaderType::Pixel);
					}
				);
			}
			else
			{
				m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_pStates->NonPremultiplied());
			}
		}

		std::visit([this](auto& c) {

			// decayを使用し参照を外した素の型を取得
			using T = std::decay_t<decltype(c)>;

			// SpriteCommandの場合
			if constexpr (std::is_same_v<T, DrawSpriteCommand>)
			{
				// テクスチャがあれば描画
				if (c.pTexture)
					m_spriteBatch->Draw(c.pTexture, c.pos, (c.srcRect ? &c.srcRect.value() : nullptr), c.color, c.angle, c.origin, c.scale);
			}
			// TextCommandの場合
			if constexpr (std::is_same_v<T, DrawTextCommand>)
			{
				// フォントがあれば描画
				if (c.spriteFont)
					c.spriteFont->DrawString(m_spriteBatch.get(), c.text.c_str(), c.pos, c.color, c.angle, c.origin, c.scale);
			}

		}, command);
	}

	// 終了
	m_spriteBatch->End();
}

void REngine::DrawCommandExecutor::DrawModelCommandExecute(const std::vector<DrawModelCommand>& commands)
{
	// 全コマンドを描画
	for (auto& c : commands)
	{
		// マテリアルを取得
		auto* material = m_assetManager.Get(c.material);

		// マテリアルが設定されていて有効なとき
		if (material && material->IsValid())
		{
			// InfoManager経由のPerObjectバッファ更新
			m_infoManager.UpdatePerObject(c.world);
			m_infoManager.BindPerObject();

			// 定数バッファを更新
			material->UpdateConstantBuffers(m_pDevice, m_pContext);

			// InputLayoutを取得
			auto inputLayout = GraphicsManager::Instance().GetInputLayoutCache().GetOrCreate(
				m_pDevice, static_cast<uint32_t>(typeid(VertexPositionNormalTexture).hash_code()),
				material->GetBlob(ShaderType::Vertex), VertexPositionNormalTexture::GetLayout()
			);

			// なければスキップ
			if (!inputLayout) continue;

			// マテリアルの設定を適用
			material->Apply(m_pContext);

			// 頂点バッファの設定
			ID3D11Buffer* vsBuffers[] = { c.pMesh->GetVertexBuffer() };
			UINT strides[] = { sizeof(VertexPositionNormalTexture) };
			UINT offsets[] = { 0 };
			m_pContext->IASetVertexBuffers(0, 1, vsBuffers, strides, offsets);

			// カリング
			m_pContext->RSSetState(m_pStates->CullClockwise());
			
			// デプスソートを使用
			m_pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);

			// インデックスバッファの設定
			m_pContext->IASetIndexBuffer(c.pMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// トポロジーの設定 
			m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// 入力レイアウトの設定 
			m_pContext->IASetInputLayout(inputLayout.Get());

			// サブメッシュの取得
			auto& subMesh = c.pMesh->GetSubMeshes()[c.subMeshIndex];

			// 描画命令
			m_pContext->DrawIndexed(subMesh.indexCount, subMesh.indexOffset, subMesh.vertexOffset);
		}
	}
}
