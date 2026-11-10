/**
 * @file    DefaultMeshManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/09
 */

#include "pch.h"
#include "Assets/Types/Model/DefaultMeshManager.h"

#include "Assets/Types/Vertex/VertexTypes.h"

namespace RHEngine
{
	void DefaultMeshManager::Initialize(ID3D11Device* pDevice)
	{
		m_meshes[static_cast<uint32_t>(DefaultMeshType::SkyboxCube)] = std::move(CreateSkyboxCube(pDevice));
	}

	void DefaultMeshManager::Shutdown()
	{
		for (auto& mesh : m_meshes)
		{
			mesh.reset();
		}
	}

	Mesh* DefaultMeshManager::GetMesh(DefaultMeshType type)
	{
		uint32_t index = static_cast<uint32_t>(type);

		if (index >= m_meshes.size()) return nullptr;

		return m_meshes[static_cast<uint32_t>(type)].get();
	}

	std::unique_ptr<Mesh> DefaultMeshManager::CreateSkyboxCube(ID3D11Device* pDevice)
	{
		// 頂点
		std::vector<VertexPosition> vertices = {
			{ { -1, -1, -1 } },{ { -1,  1, -1 } },{ { 1,  1, -1 } },{ { 1, -1, -1 } },
			{ { -1, -1,  1 } },{ { -1,  1,  1 } },{ { 1,  1,  1 } },{ { 1, -1,  1 } },
		};

		// インデックス
		std::vector<uint32_t> indices = {
			0,1,2, 0,2,3,   // -Z
			4,6,5, 4,7,6,   // +Z
			4,5,1, 4,1,0,   // -X
			3,2,6, 3,6,7,   // +X
			1,5,6, 1,6,2,   // +Y
			4,0,3, 4,3,7,   // -Y
		};

		return CreateMesh(pDevice, vertices, indices);
	}
}	// namespace RHEngine
