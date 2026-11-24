/**
 * @file    SkyboxComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/03
 */

#include "pch.h"
#include "Components/World/Renderer/SkyboxComponent.h"

#include "Renderer/Renderer.h"
#include "Assets/Types/Model/DefaultMeshManager.h"

namespace RHEngine
{
	SkyboxComponent::SkyboxComponent(IComponentOwner* pOwner)
		: RendererBase(pOwner)
		, m_pMesh{ nullptr }
		, m_material{}
	{
		// メッシュを取得
		m_pMesh = DefaultMeshManager::Instance().GetMesh(DefaultMeshType::SkyboxSphere);

		ADD_PROPERTY(SkyboxComponent, m_material);
	}

	void SkyboxComponent::Draw(Renderer& renderer)
	{
		renderer.SetMaterial(m_material);

		renderer.Draw().Other().DrawSkybox(m_pMesh, 50.0f);

		renderer.SetMaterial(ERROR_HANDLE<MaterialAsset>);
	}
}	// namespace RHEngine
