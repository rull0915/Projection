//====================================================//
// ファイル名   : MaterialParams.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/10
//
// 概要 : マテリアルのパラメーターに関するヘッダ
//
// 更新履歴 :
// 2026/10/10 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <variant>
#include <SimpleMath.h>
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"
#include "Assets/Types/Shader/SamplerType.h"
#include "Assets/Types/Shader/ShaderParam.h"

namespace RHEngine
{
	// バッファとして存在する型を全て扱うVariant
	using MaterialParamVariant = std::variant<
		float,
		DirectX::SimpleMath::Vector2,
		DirectX::SimpleMath::Vector3,
		DirectX::SimpleMath::Vector4,
		DirectX::SimpleMath::Color,
		DirectX::SimpleMath::Matrix,
		Handle<Texture>,
		SamplerType
	>;

	//====================================================//
	// クラス宣言
	//====================================================//
	class MaterialParams
	{
	public:

		// パラメータタイプを取得する関数
		template<typename T>
		static ShaderParamType GetParamType()
		{
			if constexpr (std::is_same_v<T, float>)
				return ShaderParamType::Float;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector2>)
				return ShaderParamType::Float2;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector3>)
				return ShaderParamType::Float3;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector4>)
				return ShaderParamType::Float4;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Matrix>)
				return ShaderParamType::Float4x4;
			else if constexpr (std::is_same_v<T, Handle<Texture>>)
				return ShaderParamType::Texture2D;
			else if constexpr (std::is_same_v<T, SamplerType>)
				return ShaderParamType::Sampler;
			else return ShaderParamType::None;
		}


		// パラメータのデフォルト値を返す関数
		static MaterialParamVariant GetDefaultParam(ShaderParamType type)
		{
			switch (type)
			{
			case RHEngine::ShaderParamType::Float:
				return (float)0.0f;
			case RHEngine::ShaderParamType::Float2:
				return DirectX::SimpleMath::Vector2::Zero;
			case RHEngine::ShaderParamType::Float3:
				return DirectX::SimpleMath::Vector3::Zero;
			case RHEngine::ShaderParamType::Color:
				return DirectX::SimpleMath::Color{ 1, 1, 1, 1 };
			case RHEngine::ShaderParamType::Float4:
				return DirectX::SimpleMath::Vector4::Zero;
			case RHEngine::ShaderParamType::Float4x4:
				return DirectX::SimpleMath::Matrix::Identity;
			case RHEngine::ShaderParamType::Texture2D:
				return ERROR_HANDLE<Texture>;
			case RHEngine::ShaderParamType::Sampler:
				return SamplerType::None;
			default:
				return MaterialParamVariant{};
			}
		}
	};
}
