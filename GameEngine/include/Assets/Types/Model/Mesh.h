/**
 * @file    Mesh.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/06
 *
 * @brief   メッシュアセット
 */

#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <vector>

#include "Assets/Objects/AssetBase.h"
#include "SubMesh.h"
#include "Common/Property/Property.h"

namespace RHEngine
{
	namespace Loader { class OBJLoader; }

	/**
	 * @brief メッシュアセット
	 */
	class Mesh : public AssetBase
	{
		friend class Loader::OBJLoader;

	private:

		/** @brief 頂点バッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

		/** @brief インデックスバッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

		/** @brief サブメッシュリスト */
		std::vector<SubMesh> m_subMeshes;

		/** @brief 頂点数 */
		int m_vertexCount;

		/** @brief ポリゴン数 */
		int m_polygonCount;

		/** @brief サブメッシュ数 */
		int m_subMeshCount;

	public:

		/// @name ライフサイクル
		/// @{
		
		Mesh() = default;

		/**
		 * @param vertexBuffer 頂点バッファ
		 * @param indexBuffer インデックスバッファ
		 * @param subMeshes サブメッシュ
		 */
		Mesh(
			const Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer,
			const Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer,
			std::vector<SubMesh> subMeshes
		);

		~Mesh() = default;

		/// @}

		/**
		 * @brief プロパティリストを取得する
		 */
		std::vector<Property> GetProperties() override;

		/// @name ゲッター
		/// @{

		// サブメッシュのリスト
		const std::vector<SubMesh>& GetSubMeshes() const { return m_subMeshes; }

		// 頂点バッファ
		ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }

		// インデックスバッファ
		ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }

		/// @}
	};
}	// namespace RHEngine
