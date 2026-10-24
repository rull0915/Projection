//====================================================//
// ファイル名  : InputLayoutCache.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/09/25
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/Render/InputLayoutCache.h"
#include "Assets/System/Shader/ShaderReflection.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayoutCache::GetOrCreate(ID3D11Device* device, uint32_t vertexId, ShaderAsset* vs, const std::vector<VertexElementInfo>& layoutInfo)
	{
		if (!vs) return nullptr;

		// キーを生成
		uint64_t key = (static_cast<uint64_t>(vertexId) << 32 | static_cast<uint64_t>(vs->GetShaderID()));

		// キーを検索
		auto it = m_inputLayoutCache.find(key);

		// あれば
		if (it != m_inputLayoutCache.end())
		{
			// 既に作成済みのInputLayoutを返す
			return it->second;
		}

		// まだ存在しないキーなら作成
		Microsoft::WRL::ComPtr<ID3D11InputLayout> newLayout;
		HRESULT hr = Reflection::CreateInputLayout(device, vs->GetBlob(), layoutInfo, newLayout);

		// 成功すれば
		if (SUCCEEDED(hr))
		{
			// キャッシュを更新
			m_inputLayoutCache[key] = newLayout;
		}

		// 作成したInputLayoutを返す
		return newLayout;
	}
}
