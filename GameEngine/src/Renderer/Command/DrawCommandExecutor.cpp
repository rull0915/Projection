/**
 * @file    DrawCommandExecutor.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 */

#include "pch.h"
#include "Renderer/Command/DrawCommandExecutor.h"

#include "System/Render/GraphicsManager.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/AssetManager.h"
#include "Assets/Types/Vertex/VertexTypes.h"
#include "Assets/EngineDefaultAssets.h"

namespace RHEngine
{
	DrawCommandExecutor::DrawCommandExecutor(AssetManager& assetManager, RenderInfoManager& infoManager)
		: m_spriteBatch{}
		, m_infoManager{ infoManager }
		, m_pDevice{ nullptr }
		, m_pContext{ nullptr }
		, m_pStates{ nullptr }
		, m_assetManager{ assetManager }
		, m_defaultMaterial{}
	{}

	DrawCommandExecutor::~DrawCommandExecutor() = default;

	void DrawCommandExecutor::Initialize()
	{
		// デバイスリソースの取得
		DX::DeviceResources* dr = GraphicsManager::Instance().GetDeviceResources();

		m_pDevice = dr->GetD3DDevice();

		m_pContext = dr->GetD3DDeviceContext();

		m_pStates = dr->GetCommonStates();

		// 動的頂点バッファの初期化
		m_dynamicVertexBuffer.Initialize(
			m_pDevice, DrawPrimitiveCommand::MAX_VERTEX_COUNT, sizeof(VertexPositionColorTexture)
		);

		// デフォルトマテリアルのロード
		m_defaultMaterial = m_assetManager.LoadFromUUID(DefaultAsset::PRIMITIVE_MATERIAL).As<MaterialAsset>();

		// スプライトバッチの初期化
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pContext);
	}

	void DrawCommandExecutor::DrawCommandExecute(DrawCommandContainer& container)
	{
		// 各コマンドの実行
		DrawMeshCommandExecute(container.GetDrawMeshCommands());
		DrawPrimitiveCommandExecute(container.GetDrawPrimitiveCommands());
		DrawSpriteAndFontCommandExecute(container.GetDrawUICommands());
	}

	void DrawCommandExecutor::DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands)
	{
		// Discard要求
		m_dynamicVertexBuffer.RequestDiscard();

		// 頂点バッファをバインド
		m_dynamicVertexBuffer.Bind(m_pContext);

		// カリング不使用
		m_pContext->RSSetState(m_pStates->CullNone());

		// コマンドの描画情報をキャッシュする構造体
		struct DrawBatchInfo
		{
			uint32_t startVertex;	///< インデックス
			uint32_t vertexCount;	///< 頂点数
			D3D11_PRIMITIVE_TOPOLOGY topology;	///< トポロジー
			MaterialAsset* material;	///< マテリアル
			DirectX::SimpleMath::Matrix world;	///< ワールド行列
		};

		// 1描画の情報リスト
		std::vector<DrawBatchInfo> batchList;
		batchList.reserve(commands.size());

		// コマンド分ループ
		for (auto& c : commands)
		{
			// 頂点が存在しなければ次へ
			if (c.vertices.empty()) continue;

			// マテリアルを調べる
			MaterialAsset* material = m_assetManager.Get<MaterialAsset>(c.material);

			// なければデフォルトマテリアルを使用
			if (!material)
			{
				material = m_assetManager.Get(m_defaultMaterial);
			}

			// 頂点情報を書き込む
			uint32_t startVertex = m_dynamicVertexBuffer.Write(m_pContext, c.vertices.data(), c.vertices.size());

			// 描画コマンドバッチを追加
			batchList.push_back({
				startVertex,
				static_cast<uint32_t>(c.vertices.size()),
				c.topology,
				material,
				c.world
				});
		}

		if (batchList.empty()) return;

		// 1バッチを描画する
		auto drawBatch = [&](DrawBatchInfo batch)
			{
				// PerObjectバッファを更新
				m_infoManager.UpdatePerObject(batch.world);
				m_infoManager.BindPerObject();

				// 有効マテリアルなら
				if (batch.material && batch.material->IsValid())
				{
					// 適用できなければ終了
					if (!batch.material->Apply(m_pContext)) return;

					// InputLayoutを取得
					auto inputLayout = GraphicsManager::Instance().GetInputLayoutCache().GetOrCreate(
						m_pDevice, static_cast<uint32_t>(typeid(VertexPositionColorTexture).hash_code()),
						batch.material->GetShaderAsset(ShaderType::Vertex), VertexPositionColorTexture::GetLayout()
					);

					// InputLayoutを設定
					m_pContext->IASetInputLayout(inputLayout.Get());

					// 定数バッファの更新
					batch.material->UpdateConstantBuffers(m_pDevice, m_pContext);
				}

				// トポロジーを設定
				m_pContext->IASetPrimitiveTopology(batch.topology);

				// 描画
				m_pContext->Draw(batch.vertexCount, batch.startVertex);
			};

		//-------------- 遮蔽部分を半透明描画 --------------//
		{
			// GREATERの深度ステンシルステート
			m_pContext->OMSetDepthStencilState(
				GraphicsManager::Instance().GetDepthStencilList().GetState(DepthStencilType::DepthGreater).Get(), 0);

			// 半透明ブレンドを設定
			float blendFactor[4] = { 0.3f, 0.3f, 0.3f, 0.3f }; // 全体的に薄く乗算
			m_pContext->OMSetBlendState(
				GraphicsManager::Instance().GetBlendList().GetState(BlendType::BlendFactorRGB).Get()
				, blendFactor, 0xFFFFFFFF);

			for (auto& batch : batchList)
			{
				drawBatch(batch);
			}
		}

		//-------------- 可視部分を通常描画 --------------//
		{
			// デフォルトの深度ステンシルステート
			m_pContext->OMSetDepthStencilState(
				GraphicsManager::Instance().GetDepthStencilList().GetState(DepthStencilType::Default).Get(), 0);

			// 通常の非透過ブレンド
			m_pContext->OMSetBlendState(
				GraphicsManager::Instance().GetBlendList().GetState(BlendType::PremultipliedAlpha).Get(),
				nullptr, 0xFFFFFFFF);
			
			// 描画
			for (auto& batch : batchList)
			{
				drawBatch(batch);
			}
		}
	}

	void DrawCommandExecutor::DrawSpriteAndFontCommandExecute(const std::vector<DrawCommandContainer::DrawUICommand>& commands)
	{
		// スプライトバッチの開始
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, 
			GraphicsManager::Instance().GetBlendList().GetState(BlendType::AlphaBlend).Get()
		);

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
						GraphicsManager::Instance().GetBlendList().GetState(BlendType::AlphaBlend).Get(),
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
					m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, GraphicsManager::Instance().GetBlendList().GetState(BlendType::AlphaBlend).Get());
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

	void DrawCommandExecutor::DrawMeshCommandExecute(const std::vector<DrawMeshCommand>& commands)
	{
		// カリング
		m_pContext->RSSetState(m_pStates->CullClockwise());

		// デプスソートを使用
		m_pContext->OMSetDepthStencilState(
			GraphicsManager::Instance().GetDepthStencilList().GetState(DepthStencilType::Default).Get(), 0);

		// トポロジーの設定 
		m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
					material->GetShaderAsset(ShaderType::Vertex), VertexPositionNormalTexture::GetLayout()
				);

				// なければスキップ
				if (!inputLayout) continue;

				// マテリアルの設定を適用
				if (!material->Apply(m_pContext)) continue;

				// 頂点バッファの設定
				ID3D11Buffer* vsBuffers[] = { c.pMesh->GetVertexBuffer() };
				UINT strides[] = { sizeof(VertexPositionNormalTexture) };
				UINT offsets[] = { 0 };
				m_pContext->IASetVertexBuffers(0, 1, vsBuffers, strides, offsets);

				// インデックスバッファの設定
				m_pContext->IASetIndexBuffer(c.pMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

				// 入力レイアウトの設定 
				m_pContext->IASetInputLayout(inputLayout.Get());

				// サブメッシュの取得
				auto& subMesh = c.pMesh->GetSubMeshes()[c.subMeshIndex];

				// 描画命令
				m_pContext->DrawIndexed(subMesh.indexCount, subMesh.indexOffset, subMesh.vertexOffset);
			}
		}
	}
}	// namespace RHEngine
