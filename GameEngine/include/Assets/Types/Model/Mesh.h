//====================================================//
// ファイル名   : Mesh.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/06
//
// 概要 : メッシュアセット
//
// 更新履歴 :
// 2026/10/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <wrl/client.h>
#include <d3d11.h>

#include "Assets/Objects/AssetBase.h"
#include "SubMesh.h"

namespace REngine
{
	namespace Loader
	{
		class OBJLoader;
	}

	//====================================================//
	// クラス宣言
	//====================================================//
	class Mesh : public AssetBase
	{
	public:
		// ロードクラスをフレンド指定
		friend class Loader::OBJLoader;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

		// インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

		// サブメッシュ配列
		std::vector<SubMesh> m_subMeshes;

		// 各情報の数
		int m_vertexCount;
		int m_polygonCount;
		int m_subMeshCount;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Mesh()
			: m_vertexBuffer{}
			, m_indexBuffer{}
			, m_subMeshes{}
			, m_vertexCount{}
			, m_polygonCount{}
			, m_subMeshCount{}
		{
		}
		~Mesh() = default;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// サブメッシュのリスト
		const std::vector<SubMesh>& GetSubMeshes() const { return m_subMeshes; }

		// 頂点バッファ
		ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }

		// インデックスバッファ
		ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }

		// プロパティリスト
		std::vector<Property> GetProperties() override
		{
			std::vector<Property> properties;

			Property vertexInfo;
			vertexInfo.name = "Vertex:";
			vertexInfo.type = PropertyType::Int;
			vertexInfo.typeIndex = std::type_index(typeid(int));
			vertexInfo.value = &m_vertexCount;

			Property indexInfo;
			indexInfo.name = "Polygon:";
			indexInfo.type = PropertyType::Int;
			indexInfo.typeIndex = std::type_index(typeid(int));
			indexInfo.value = &m_polygonCount;

			Property subMeshInfo;
			subMeshInfo.name = "SubMesh:";
			subMeshInfo.type = PropertyType::Int;
			subMeshInfo.typeIndex = std::type_index(typeid(int));
			subMeshInfo.value = &m_subMeshCount;

			properties.push_back(subMeshInfo);
			properties.push_back(vertexInfo);
			properties.push_back(indexInfo);

			return properties;
		}
	};
}
