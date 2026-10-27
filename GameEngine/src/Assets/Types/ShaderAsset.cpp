//====================================================//
// ファイル名  : ShaderAsset.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/03
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/Shader/ShaderAsset.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	ShaderAsset::ShaderAsset(uint32_t id)
		: m_type{ ShaderType::Vertex }
		, m_shader{}
		, m_byteCode{ nullptr }
		, m_params{}
		, m_cBuffers{}
		, m_shaderID{ id }
	{
	}

	void ShaderAsset::Bind(ID3D11DeviceContext* context)
	{
		// visitを使用してどのShaderでも対応可能に
		std::visit
		(
			[&](auto&& obj)
			{
				// decayを使用し参照を外した巣の型を取得
				using T = std::decay_t<decltype(obj)>;

				// 型によって分岐

				// 頂点シェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11VertexShader>>) 
				{
					context->VSSetShader(obj.Get(), nullptr, 0); 
				}
				// ピクセルシェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11PixelShader>>) 
				{
					context->PSSetShader(obj.Get(), nullptr, 0); 
				}
				// ジオメトリシェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11GeometryShader>>) { context->GSSetShader(obj.Get(), nullptr, 0); }
				// ハルシェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11HullShader>>) { context->HSSetShader(obj.Get(), nullptr, 0); }
				// コンピュートシェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11ComputeShader>>) { context->CSSetShader(obj.Get(), nullptr, 0); }
				// ドメインシェーダ
				if constexpr (std::is_same_v<T, Microsoft::WRL::ComPtr<ID3D11DomainShader>>) { context->DSSetShader(obj.Get(), nullptr, 0); }
			},
			// Shader本体を渡す
			m_shader
		);
	}

	ShaderParam* ShaderAsset::FindParam(const std::string& name)
	{
		// 名前が一致するパラメータを探して取得
		auto it = std::find_if(m_params.begin(), m_params.end(),
			[&](const ShaderParam& param)->bool { return param.name == name; });

		// あれば返す
		if (it != m_params.end()) return &(*it);
		else return nullptr;
	}
}
