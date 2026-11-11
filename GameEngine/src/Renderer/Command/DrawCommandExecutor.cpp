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
		DrawSkyboxCommandExecute(container.GetDrawSkyboxCommands());
		DrawWorldSpriteCommandExecute(container.GetDrawWorldSpriteCommands());
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
		m_pContext->RSSetState(GetRasterizerState(RasterizerType::CullNone));

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
			m_pContext->OMSetDepthStencilState(GetDepthStencilState(DepthStencilType::DepthGreater), 0);

			// 半透明ブレンドを設定
			float blendFactor[4] = { 0.3f, 0.3f, 0.3f, 0.3f }; // 全体的に薄く乗算
			m_pContext->OMSetBlendState(GetBlendState(BlendType::BlendFactorRGB), blendFactor, 0xFFFFFFFF);

			for (auto& batch : batchList)
			{
				drawBatch(batch);
			}
		}

		//-------------- 可視部分を通常描画 --------------//
		{
			// デフォルトの深度ステンシルステート
			m_pContext->OMSetDepthStencilState(GetDepthStencilState(DepthStencilType::Default), 0);

			// 通常の非透過ブレンド
			m_pContext->OMSetBlendState(GetBlendState(BlendType::PremultipliedAlpha), nullptr, 0xFFFFFFFF);
			
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
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, GetBlendState(BlendType::AlphaBlend));

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

				// アセットのPSが使用可能か調べるラムダ
				auto isPsReady = [this](MaterialAsset* m)
					{
						if (!m || !m->IsValid()) return false;	// 無効アセットならfalse
						auto* ps = m->GetShaderAsset(ShaderType::Pixel);	// PSが取得できなければfalse
						return ps && ps->GetStatus() == LoadStatus::Loaded;	// PSのロードが未完了ならfalse
					};

				// 再度Begin
				if (isPsReady(mat))
				{
					m_spriteBatch->Begin(
						DirectX::SpriteSortMode_Deferred,
						GetBlendState(BlendType::AlphaBlend),
						nullptr,
						nullptr,
						nullptr,
						[this, mat]()
						{
							if (!mat->BindShader(m_pContext, ShaderType::Pixel))
								m_pContext->PSSetShader(nullptr, nullptr, 0);
						}
					);
				}
				else
				{
					m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, GetBlendState(BlendType::AlphaBlend));
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
		m_pContext->RSSetState(GetRasterizerState(RasterizerType::Default));

		// デプスソートを使用
		m_pContext->OMSetDepthStencilState(GetDepthStencilState(DepthStencilType::Default), 0);

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
	
	void DrawCommandExecutor::DrawSkyboxCommandExecute(const std::vector<DrawSkyboxCommand>& commands)
	{
		// 不透明ブレンド
		m_pContext->OMSetBlendState(GetBlendState(BlendType::Opaque), nullptr, 0xFFFFFFFF);

		// スカイボックス用の深度ステンシルステート
		m_pContext->OMSetDepthStencilState(GetDepthStencilState(DepthStencilType::SkyBox), 0);

		// 逆面をカリング
		m_pContext->RSSetState(GetRasterizerState(RasterizerType::CullFront));

		// トポロジーを設定
		m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// カメラの位置を取得
		DirectX::SimpleMath::Vector3 cameraPos = m_infoManager.GetCurrentCameraData().position;

		for (auto& command : commands)
		{
			// メッシュが不正の場合
			if (!command.pMesh || command.pMesh->GetSubMeshes().empty()) continue;

			// マテリアルの取得
			auto* material = m_assetManager.Get(command.material);
			if (!material || !material->IsValid()) continue;
			
			// ワールド行列を作成
			DirectX::SimpleMath::Matrix world =
				DirectX::SimpleMath::Matrix::CreateScale(command.scale) *
				DirectX::SimpleMath::Matrix::CreateTranslation(cameraPos);

			// PerObjectバッファに設定
			m_infoManager.UpdatePerObject(world);
			m_infoManager.BindPerObject();

			// 定数バッファの更新
			material->UpdateConstantBuffers(m_pDevice, m_pContext);

			// InputLayoutを取得
			auto inputLayout = GraphicsManager::Instance().GetInputLayoutCache().GetOrCreate(
				m_pDevice, static_cast<uint32_t>(typeid(VertexPosition).hash_code()),
				material->GetShaderAsset(ShaderType::Vertex), VertexPosition::GetLayout());

			if (!inputLayout) continue;

			// バインド
			if (!material->Apply(m_pContext)) continue;

			// 頂点バッファをバインド
			ID3D11Buffer* vb[] = { command.pMesh->GetVertexBuffer() };
			UINT stride = sizeof(VertexPosition), offset = 0;
			m_pContext->IASetVertexBuffers(0, 1, vb, &stride, &offset);

			// インデックスバッファをバインド
			m_pContext->IASetIndexBuffer(command.pMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// インプットレイアウトをバインド
			m_pContext->IASetInputLayout(inputLayout.Get());

			// 0番のサブメッシュのみを使用し描画
			auto& sub = command.pMesh->GetSubMeshes()[0];
			m_pContext->DrawIndexed(sub.indexCount, sub.indexOffset, sub.vertexOffset);
		}
	}

	void DrawCommandExecutor::DrawWorldSpriteCommandExecute(const std::vector<DrawWorldSpriteCommand>& commands)
	{
		// 半透明ブレンド
		m_pContext->OMSetBlendState(GetBlendState(BlendType::AlphaBlend), nullptr, 0xFFFFFFFF);

		// 通常の深度ステンシルステート
		m_pContext->OMSetDepthStencilState(GetDepthStencilState(DepthStencilType::Default), 0);

		// 逆面をカリング
		// @TODO 指定できるように
		m_pContext->RSSetState(GetRasterizerState(RasterizerType::CullNone));

		// トポロジーを設定
		m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (auto& command : commands)
		{
			// テクスチャがなければ次へ
			if (!command.pTexture) continue;

			// マテリアルの取得
			auto* material = m_assetManager.Get(command.material);
			if (!material || !material->IsValid())
			{
				// なければWorldSprite用デフォルトマテリアルを使用する
				static auto s_defaultHandle = m_assetManager.LoadFromUUID(DefaultAsset::SPRITE_MATERIAL).As<MaterialAsset>();
				material = m_assetManager.Get<MaterialAsset>(s_defaultHandle);

				if (!material) continue;
 
				// デフォルトマテリアル用の情報をバインドする
				material->SetParam(ShaderType::Vertex, "uv", command.uv);
				material->SetParam(ShaderType::Pixel, "color", command.color);

				m_pContext->PSSetShaderResources(11, 1, &command.pTexture);
				m_pContext->PSSetSamplers(11, 1, GraphicsManager::Instance().GetSamplerList().GetSampler(SamplerType::LinearWrap).GetAddressOf());
			}

			material->UpdateConstantBuffers(m_pDevice, m_pContext);
			if (!material->Apply(m_pContext)) continue;

			// PerObjectバッファに設定
			m_infoManager.UpdatePerObject(command.world);
			m_infoManager.BindPerObject();

			// InputLayoutを取得
			auto inputLayout = GraphicsManager::Instance().GetInputLayoutCache().GetOrCreate(
				m_pDevice, static_cast<uint32_t>(typeid(VertexPositionNormalTexture).hash_code()),
				material->GetShaderAsset(ShaderType::Vertex), VertexPositionNormalTexture::GetLayout());

			if (!inputLayout) continue;

			// 頂点バッファをバインド
			ID3D11Buffer* vb[] = { command.pMesh->GetVertexBuffer() };
			UINT stride = sizeof(VertexPositionNormalTexture), offset = 0;
			m_pContext->IASetVertexBuffers(0, 1, vb, &stride, &offset);

			// インデックスバッファをバインド
			m_pContext->IASetIndexBuffer(command.pMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// インプットレイアウトをバインド
			m_pContext->IASetInputLayout(inputLayout.Get());

			// 0番のサブメッシュのみを使用し描画
			auto& sub = command.pMesh->GetSubMeshes()[0];
			m_pContext->DrawIndexed(sub.indexCount, sub.indexOffset, sub.vertexOffset);
		}
	}
}	// namespace RHEngine
