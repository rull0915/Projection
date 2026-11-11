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
	namespace
	{
		using Vec3 = DirectX::SimpleMath::Vector3;
		using Vec2 = DirectX::SimpleMath::Vector2;

		/**
		 * @brief UV球の頂点とインデックスを生成する
		 * @param radius 半径
		 * @param slices 上下分割数
		 * @param stacks 左右分割数
		 * @param outVertices 頂点リストを返す用の変数
		 * @param outIndices インデックスリストを返す用の変数
		 */
		void GenerateSphere(float radius, uint32_t slices, uint32_t stacks,
			std::vector<VertexPositionNormalTexture>& outVertices, std::vector<uint32_t>& outIndices)
		{
			outVertices.reserve((slices + 1) * (stacks + 1));
			outIndices.reserve(slices * stacks * 6);

			// 頂点
			for (uint32_t i = 0; i <= stacks; ++i)
			{
				float phi = DirectX::XM_PI * static_cast<float>(i) / stacks;	// 0(上)〜π(下)

				for (uint32_t j = 0; j <= slices; ++j)
				{
					float theta = DirectX::XM_2PI * static_cast<float>(j) / slices;

					// 角度を使って中心点から点へのベクトルを作成
					Vec3 n(std::sin(phi) * std::cos(theta), std::cos(phi), std::sin(phi) * std::sin(theta));

					// 分割の割合をUVとして使用
					Vec2 uv(static_cast<float>(j) / slices, static_cast<float>(i) / stacks);

					// 頂点リストに追加
					outVertices.push_back({ n * radius, n, uv });
				}
			}

			// インデックス
			for (uint32_t i = 0; i < stacks; ++i)
			{
				for (uint32_t j = 0; j < slices; ++j)
				{
					uint32_t a = i * (slices + 1) + j;	// 左上
					uint32_t b = a + slices + 1;		// 左下

					outIndices.insert(outIndices.end(), { a, a + 1, b,   a + 1, b + 1, b });
				}
			}
		}
	}

	void DefaultMeshManager::Initialize(ID3D11Device* pDevice)
	{
		// タイプとメッシュからリストを増やすラムダ
		auto set = [&](DefaultMeshType type, std::unique_ptr<Mesh> mesh)
			{
				m_meshes[static_cast<uint32_t>(type)] = std::move(mesh);
			};

		set(DefaultMeshType::SkyboxCube, CreateSkyboxCube(pDevice));
		set(DefaultMeshType::SkyboxSphere, CreateSkyboxSphere(pDevice));
		set(DefaultMeshType::Cube, CreateCube(pDevice));
		set(DefaultMeshType::Quad, CreateQuad(pDevice));
		set(DefaultMeshType::Sphere, CreateSphere(pDevice));
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

	std::unique_ptr<Mesh> DefaultMeshManager::CreateSkyboxSphere(ID3D11Device* pDevice)
	{
		std::vector<VertexPositionNormalTexture> full;
		std::vector<uint32_t> indices;

		// 球を作成
		GenerateSphere(0.5f, 32, 16, full, indices);

		// 位置のみに変換
		std::vector<VertexPosition> vertices;
		vertices.reserve(full.size());
		for (auto& v : full) vertices.push_back({ v.position });

		return CreateMesh(pDevice, vertices, indices);
	}

	std::unique_ptr<Mesh> DefaultMeshManager::CreateSphere(ID3D11Device* pDevice)
	{
		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint32_t> indices;
		GenerateSphere(0.5f, 32, 16, vertices, indices);	// 直径1

		return CreateMesh(pDevice, vertices, indices);
	}

	std::unique_ptr<Mesh> DefaultMeshManager::CreateCube(ID3D11Device* pDevice)
	{
		// 面ごとの法線と「上」方向(法線に直交する軸)
		struct Face { Vec3 normal; Vec3 up; };
		const Face faces[] = {
			{ { 1, 0, 0 },{ 0, 1, 0 } },	// +X
			{ { -1, 0, 0 },{ 0, 1, 0 } },	// -X
			{ { 0,  1, 0 },{ 0, 0, 1 } },	// +Y
			{ { 0, -1, 0 },{ 0, 0, 1 } },	// -Y
			{ { 0, 0,  1 },{ 0, 1, 0 } },	// +Z
			{ { 0, 0, -1 },{ 0, 1, 0 } },	// -Z
		};

		const float h = 0.5f;	// 半辺長(1辺 = 1)

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint32_t> indices;
		vertices.reserve(24);
		indices.reserve(36);

		for (const auto& f : faces)
		{
			// 外側から見て右方向(左手系: right = up × forward、forward = -normal)
			Vec3 right = f.up.Cross(-f.normal);

			uint32_t base = static_cast<uint32_t>(vertices.size());

			// 左下、左上、右上、右下
			vertices.push_back({ (f.normal - right - f.up) * h, f.normal,{ 0, 1 } });
			vertices.push_back({ (f.normal - right + f.up) * h, f.normal,{ 0, 0 } });
			vertices.push_back({ (f.normal + right + f.up) * h, f.normal,{ 1, 0 } });
			vertices.push_back({ (f.normal + right - f.up) * h, f.normal,{ 1, 1 } });

			indices.insert(indices.end(), { base, base + 1, base + 2,   base, base + 2, base + 3 });
		}

		return CreateMesh(pDevice, vertices, indices);
	}

	std::unique_ptr<Mesh> DefaultMeshManager::CreateQuad(ID3D11Device* pDevice)
	{
		// XY平面、+Z方向(カメラ側)を向く 1x1 の板
		const Vec3 n(0, 0, 1);

		std::vector<VertexPositionNormalTexture> vertices = {
			{ { -0.5f, -0.5f, 0 }, n,{ 0, 1 } },	// 左下
			{ { -0.5f,  0.5f, 0 }, n,{ 0, 0 } },	// 左上
			{ { 0.5f,  0.5f, 0 }, n,{ 1, 0 } },	// 右上
			{ { 0.5f, -0.5f, 0 }, n,{ 1, 1 } },	// 右下
		};

		std::vector<uint32_t> indices = { 0, 1, 2,  0, 2, 3 };

		return CreateMesh(pDevice, vertices, indices);
	}
}	// namespace RHEngine
