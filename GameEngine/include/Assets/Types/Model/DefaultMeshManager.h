/**
 * @file    DefaultMeshManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/09
 *
 * @brief   デフォルトのメッシュ形状を保持する
 */

#pragma once

#include <array>
#include <memory>
#include <cstdint>
#include <vector>
#include <d3d11.h>
#include <Windows.h>

#include "Mesh.h"
#include "Assets/Types/Vertex/VertexTypes.h"

namespace RHEngine
{
	enum class DefaultMeshType : uint32_t
	{
		SkyboxCube,     // Position のみ(スカイボックス用)
		SkyboxSphere,   // Position のみ(スカイボックス用)
		Cube,           // PositionNormalTexture
		Quad,           // PositionNormalTexture
		Sphere,         // PositionNormalTexture
		Count
	};

	/**
	 * @brief デフォルトのメッシュ形状を保持する
	 */
	class DefaultMeshManager
	{
	private:

		/** @brief 各デフォルトメッシュ */
		std::array<std::unique_ptr<Mesh>, static_cast<uint32_t>(DefaultMeshType::Count)> m_meshes;

	private:

		/// @name ライフサイクル
		/// @{

		DefaultMeshManager() = default;

		~DefaultMeshManager() = default;

		/// @}

	public:

		/**
		 * @brief インスタンスを返す
		 * @return 静的なインスタンス
		 */
		static DefaultMeshManager& Instance()
		{
			static DefaultMeshManager instance;
			return instance;
		}

		/**
		 * @brief 各メッシュを生成する
		 * @param pDevice デバイスポインタ
		 */
		void Initialize(ID3D11Device* pDevice);

		/**
		 * @brief 各メッシュを破棄する
		 */
		void Shutdown();

		/**
		 * @brief メッシュを取得する
		 * @param type メッシュタイプ
		 * @return メッシュポインタ
		 */
		Mesh* GetMesh(DefaultMeshType type);

	private:

		static std::unique_ptr<Mesh> CreateSkyboxCube(ID3D11Device* pDevice);
		static std::unique_ptr<Mesh> CreateSkyboxSphere(ID3D11Device* pDevice);
		static std::unique_ptr<Mesh> CreateCube(ID3D11Device* pDevice);
		static std::unique_ptr<Mesh> CreateQuad(ID3D11Device* pDevice);
		static std::unique_ptr<Mesh> CreateSphere(ID3D11Device* pDevice);

		/**
		 * @brief 頂点とインデックスからメッシュを作成する
		 * @tparam T 頂点型
		 * @param pDevice デバイスポインタ
		 * @param vertices 頂点リスト
		 * @param indices インデックスリスト
		 * @return メッシュ
		 */
		template<VertexType T>
		static std::unique_ptr<Mesh> CreateMesh(ID3D11Device* pDevice, const std::vector<T>& vertices, const std::vector<uint32_t>& indices);
	};

	template<VertexType T>
	inline std::unique_ptr<Mesh> DefaultMeshManager::CreateMesh(ID3D11Device* pDevice, const std::vector<T>& vertices, const std::vector<uint32_t>& indices)
	{
		// どちらかが空なら作成しない
		if (vertices.empty() || indices.empty()) return nullptr;

		// 頂点バッファ用デスクを生成
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());   // バッファのサイズ
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // 頂点バッファとして使用
		desc.CPUAccessFlags = 0;                        // CPU からアクセスは不要
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices.data();	// CPU 側のデータのポインタを渡す
		initData.SysMemPitch = 0;           // 頂点バッファの場合、ピッチは不要（ 0 にする）
		initData.SysMemSlicePitch = 0;      // 頂点バッファの場合、スライスも不要（ 0 にする）

		// 頂点バッファを作成
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		DX::ThrowIfFailed(
			pDevice->CreateBuffer(&desc, &initData, vertexBuffer.ReleaseAndGetAddressOf())
		);

		// インデックスバッファ用デスクを作成 頂点バッファで使ったものを上書して再利用
		desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());    // バッファのサイズ
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      // インデックスバッファとして使用

		initData.pSysMem = indices.data(); // CPU 側のデータのポインタを渡す

		// インデックスバッファを作成
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		DX::ThrowIfFailed(
			pDevice->CreateBuffer(&desc, &initData, indexBuffer.ReleaseAndGetAddressOf())
		);

		std::vector<SubMesh> subMeshes{};
		subMeshes.emplace_back(static_cast<UINT>(indices.size()), 0, 0);

		return std::make_unique<Mesh>(
			std::move(vertexBuffer),
			std::move(indexBuffer),
			std::move(subMeshes));
	}
}	// namespace RHEngine
