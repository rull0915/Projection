/**
 * @file    SpriteRenderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/10
 */

#include "pch.h"
#include "Components/World/Renderer/SpriteRenderer.h"

#include "Assets/Types/Model/DefaultMeshManager.h"
#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"

namespace RHEngine
{
	SpriteRenderer::SpriteRenderer(IComponentOwner* pOwner)
		: RendererBase(pOwner)
		, m_texture{}
		, m_color{ 1.0f, 1.0f, 1.0f, 1.0f }
		, m_minUvRect{ 0.0f, 0.0f }
		, m_maxUvRect{ 1.0f, 1.0f }
		, m_mode{ RenderMode::World }
		, m_pMesh{ nullptr }
		, m_fixedAxis{ 0.0f, 1.0f, 0.0f }
		, m_pAssetManager{ nullptr }
	{
		// Quadメッシュを取得
		m_pMesh = DefaultMeshManager::Instance().GetMesh(DefaultMeshType::Quad);

		ADD_PROPERTY(SpriteRenderer, m_texture);
		ADD_PROPERTY(SpriteRenderer, m_color);
		ADD_PROPERTY(SpriteRenderer, m_minUvRect);
		ADD_PROPERTY(SpriteRenderer, m_maxUvRect);
		ADD_PROPERTY(SpriteRenderer, m_mode);
		ADD_PROPERTY(SpriteRenderer, m_fixedAxis);
	}

	void SpriteRenderer::OnValidate()
	{
		// 最小UVと最大UVの関係になるように補正
		m_minUvRect.x = std::clamp(m_minUvRect.x, 0.0f, 0.999f);
		m_maxUvRect.x = std::clamp(m_maxUvRect.x, 0.001f, 1.0f);
		if (m_minUvRect.x > m_maxUvRect.x) m_minUvRect.x = m_maxUvRect.x - 0.001f;
		
		m_minUvRect.y = std::clamp(m_minUvRect.y, 0.0f, 0.999f);
		m_maxUvRect.y = std::clamp(m_maxUvRect.y, 0.001f, 1.0f);
		if (m_minUvRect.y > m_maxUvRect.y) m_minUvRect.y = m_maxUvRect.y - 0.001f;

		// 固定軸を正規化
		m_fixedAxis.Normalize();
	}

	void SpriteRenderer::Draw(Renderer& renderer)
	{
		if (!m_pAssetManager) return;

		// テクスチャを取得する
		auto pTexture = m_pAssetManager->Get(m_texture);
		if (!pTexture) return;

		// World行列を作成する
		DirectX::SimpleMath::Matrix world{};

		// モードによって変更
		switch (m_mode)
		{
			// ワールドの場合
		case RHEngine::SpriteRenderer::RenderMode::World:
			world = GetTransform()->GetWorldMatrix();
			break;

			// ビルボードの場合
		case RHEngine::SpriteRenderer::RenderMode::Billboard: {

			// カメラの情報を取得
			auto& cameraData = renderer.GetCurrentCameraInfo();

			auto f = -cameraData.forward;

			world =
				GetTransform()->GetWorldScaleMatrix() *			// 拡縮行列
				DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XMConvertToRadians(180.0f)) *	// 反転するため180回転 (@TODO 反転の原因解明)
				DirectX::SimpleMath::Matrix::CreateBillboard(	// ビルボード行列 (移動 + 回転)
					GetTransform()->GetWorldPosition(), cameraData.position, cameraData.up
				);
			break;
		}
			// 軸固定ビルボードの場合
		case RHEngine::SpriteRenderer::RenderMode::ConstrainedBillboard: {

			// カメラの情報を取得
			auto& cameraData = renderer.GetCurrentCameraInfo();

			world =
				GetTransform()->GetWorldScaleMatrix() *
				DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XMConvertToRadians(180.0f)) *
				DirectX::SimpleMath::Matrix::CreateConstrainedBillboard(
					GetTransform()->GetWorldPosition(), cameraData.position, m_fixedAxis, &cameraData.forward
				);
			break;
		}
		default:
			break;
		}

		// 描画コマンドを発行
		renderer.Draw().Sprite().DrawInWorld(
			pTexture->Get(), m_pMesh, world, { m_minUvRect.x, m_minUvRect.y, m_maxUvRect.x, m_maxUvRect.y }, m_color
		);
	};
}	// namespace RHEngine
