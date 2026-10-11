//====================================================//
// ファイル名   : MaterialConstant.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/10
//
// 概要 : マテリアルのセーブロードで使用する定数をまとめたヘッダ
//
// 更新履歴 :
// 2026/10/10 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <utility>
#include <string_view>

#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "ThirdParty/nlohmann/json.hpp"
#include "ThirdParty/magic_enum/magic_enum.hpp"
#include "Assets/Managers/AssetContext.h"
#include "Assets/Types/Shader/ShaderParam.h"
#include "Assets/Types/Shader/SamplerType.h"
#include "Assets/Types/Texture.h"
#include "Assets/Objects/Handle.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class MaterialConstant
	{
	public:

		// シェーダー名とシェーダータイプの対応表
		static constexpr std::pair<std::string_view, ShaderType> SHADER_NAME_TYPE[] =
		{
			{ "VertexShader", ShaderType::Vertex },
			{ "PixelShader", ShaderType::Pixel },
			{ "HullShader", ShaderType::Hull },
			{ "DomainShader", ShaderType::Domain },
			{ "GeometryShader", ShaderType::Geometry },
			{ "ComputeShader", ShaderType::Compute },
		};

		// jsonからパラメータ
		static MaterialParamVariant GetVariant(const AssetContext& ctx, ShaderParamType type, const nlohmann::json& json)
		{
			switch (type)
			{
				// float
			case REngine::ShaderParamType::Float:
				return static_cast<float>(json);
				// Vector2
			case REngine::ShaderParamType::Float2:
				return DirectX::SimpleMath::Vector2{ json[0], json[1] };
				// Vector3
			case REngine::ShaderParamType::Float3:
				return DirectX::SimpleMath::Vector3{ json[0], json[1], json[2] };
				// Vector4
			case REngine::ShaderParamType::Float4:
				return DirectX::SimpleMath::Vector4{ json[0], json[1], json[2], json[3] };
				// Color
			case REngine::ShaderParamType::Color:
				return DirectX::SimpleMath::Color{ json[0], json[1], json[2], json[3] };
				// Matrix
			case REngine::ShaderParamType::Float4x4:
				return DirectX::SimpleMath::Matrix{ json[0], json[1], json[2], json[3],	json[4], json[5], json[6], json[7],	json[8], json[9], json[10], json[11], json[12], json[13], json[14], json[15] };
				// Handle<Texture>
			case REngine::ShaderParamType::Texture2D:
				return ctx.LoadFromUUID(json).As<Texture>();
				// SamplerType
			case REngine::ShaderParamType::Sampler:
			{
				auto opt = magic_enum::enum_cast<SamplerType>(static_cast<std::string_view>(json));
				if (!opt) return SamplerType::None;
				return opt.value();
			}
			// その他はデフォルト値を返す
			default:
				return MaterialParamVariant{};
			}
		};

		// パラメーターからjson
		static nlohmann::json GetJson(const AssetContext& ctx, MaterialAsset::Parameter param)
		{
			nlohmann::json paramJson{};

			switch (param.type)
			{
				// float
			case REngine::ShaderParamType::Float:
				paramJson = *std::get_if<float>(&param.value);
				break;

				// Vector2
			case REngine::ShaderParamType::Float2: {
				DirectX::SimpleMath::Vector2 vec2 = *std::get_if<DirectX::SimpleMath::Vector2>(&param.value);
				paramJson = { vec2.x, vec2.y };
				break;
			}

				// Vector3
			case REngine::ShaderParamType::Float3: {
				DirectX::SimpleMath::Vector3 vec3 = *std::get_if<DirectX::SimpleMath::Vector3>(&param.value);
				paramJson = { vec3.x, vec3.y, vec3.z };
				break;
			}

				// Vector4
			case REngine::ShaderParamType::Float4: {
				DirectX::SimpleMath::Vector4 vec4 = *std::get_if<DirectX::SimpleMath::Vector4>(&param.value);
				paramJson = { vec4.x, vec4.y, vec4.z, vec4.w };
				break;
			}

				// Color
			case REngine::ShaderParamType::Color: {
				DirectX::SimpleMath::Color color = *std::get_if<DirectX::SimpleMath::Color>(&param.value);
				paramJson = { color.x, color.y, color.z, color.w };
				break;
			}

				// Matrix
			case REngine::ShaderParamType::Float4x4: {
				DirectX::SimpleMath::Matrix mat = *std::get_if<DirectX::SimpleMath::Matrix>(&param.value);
				paramJson = { mat._11, mat._12, mat._13, mat._14, mat._21, mat._22, mat._23, mat._24, mat._31, mat._32, mat._33, mat._34, mat._41, mat._42, mat._43, mat._44 };
				break;
			}

				// Handle<Texture>
			case REngine::ShaderParamType::Texture2D:
				paramJson = ctx.GetRegistry().GetUUID(std::get_if<Handle<Texture>>(&param.value)->GetUnTypeHandle());
				break;

				// SamplerType
 			case REngine::ShaderParamType::Sampler:
				paramJson = magic_enum::enum_name<SamplerType>(*std::get_if<SamplerType>(&param.value));
				break;

			default:
				break;
			}

			return paramJson;
		}
	};
}
