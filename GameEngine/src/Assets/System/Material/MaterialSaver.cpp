/**
 * @file    MaterialSaver.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/10
 */

#include "pch.h"
#include "MaterialSaver.h"

#include <fstream>

#include "MaterialConstant.h"
#include "ThirdParty/nlohmann/json.hpp"
#include "ThirdParty/magic_enum/magic_enum.hpp"
#include "Assets/Managers/AssetContext.h"

void RHEngine::Saver::MaterialSaver::Save(AssetBase* asset, const std::filesystem::path& path, AssetContext& ctx)
{
	// Materialに変換
	MaterialAsset* material = dynamic_cast<MaterialAsset*>(asset);

	if (!material) return;

	// jsonを作成
	nlohmann::json saveJson{};

	// 各シェーダーハンドルのIDを保存
	saveJson["VSHandle"] = ctx.GetRegistry().GetUUID(material->m_vertexShader.GetUnTypeHandle());
	saveJson["PSHandle"] = ctx.GetRegistry().GetUUID(material->m_pixelShader.GetUnTypeHandle());

	// 各シェーダーのパラメーターを保存
	for (auto& [name, type] : MaterialConstant::SHADER_NAME_TYPE)
	{
		// パラメータ保存用のjsonを作成
		nlohmann::json paramsJson{};

		// パラメータを取得
		auto& params = material->GetParams(type);

		// ループ
		for (auto& param : params)
		{
			// 1パラメータのjsonを作成
			nlohmann::json paramJson{};

			// パラメータ名を保存
			paramJson["Name"] = param.first;

			// パラメータタイプを保存
			std::string_view paramType = magic_enum::enum_name<ShaderParamType>(param.second.type);

			paramJson["Type"] = paramType;

			// パラメータのデータを保存
			paramJson["Value"] = MaterialConstant::GetJson(ctx, param.second);

			// jsonに追加
			paramsJson.push_back(paramJson);
		}

		// jsonに追加
		saveJson[name] = paramsJson;
	}

	// ファイルに保存
	std::ofstream ofs(path);

	if (ofs.is_open())
	{
		ofs << saveJson.dump(4);
	}

	ofs.close();
}	// namespace RHEngine
