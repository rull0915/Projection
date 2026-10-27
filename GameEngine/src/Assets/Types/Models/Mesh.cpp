//====================================================//
// ファイル名  : Mesh.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/25
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/Model/Mesh.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	std::vector<Property> RHEngine::Mesh::GetProperties()
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
}	// namespace RHEngine
