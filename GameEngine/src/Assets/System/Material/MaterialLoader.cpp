//====================================================//
// ファイル名  : MaterialLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/10
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#include <fstream>

#include "MaterialLoader.h"
#include "Assets/Managers/AssetContext.h"
#include "MaterialConstant.h"

#include "ThirdParty/nlohmann/json.hpp"
#include "Assets/Types/Shader/ShaderParam.h"
#include "ThirdParty/magic_enum/magic_enum.hpp"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Loader
	{
		LoadResult MaterialLoader::Load(const std::filesystem::path& path, AssetContext& ctx)
		{
			// リザルトの作成
			LoadResult result{};

			// Materialの生成
			MaterialAsset material{};

			// jsonに変換
			std::ifstream ifs(path);

			// 開けなかったら
			if (!ifs.is_open())
			{
				ifs.close();

				result.scuccess = false;

				return result;
			}

			// jsonから読み取り
			nlohmann::json j;
			ifs >> j;

			// 各シェーダーのハンドルを解決する
			if (j.contains("VSHandle")) 
				material.m_vertexShader = ctx.LoadFromUUID(j["VSHandle"]).As<ShaderAsset>();	// UUIDからHandleを取得
			if (j.contains("PSHandle")) 
				material.m_pixelShader = ctx.LoadFromUUID(j["PSHandle"]).As<ShaderAsset>();	// UUIDからHandleを取得

			// 各シェーダーのパラメータを取得する
			for (auto& [name, type] : MaterialConstant::SHADER_NAME_TYPE)
			{
				// キーが存在しない、または配列でない場合はスキップ
				if (!j.contains(name) || !j[name].is_array()) continue;

				// パラメータを記述したjsonを取得
				nlohmann::json shaderJson = j[name];

				// パラメータを保存するリストを取得
				auto paramsList = std::unordered_map<std::string, MaterialAsset::Parameter>{};

				// ループ
				for (auto& paramJson : shaderJson)
				{
					MaterialAsset::Parameter param{};

					// 存在しない要素があればスキップ
					if (!paramJson.contains("Type") || !paramJson.contains("Name") || !paramJson.contains("Value")) continue;

					// 列挙子名から列挙型を取得
					auto opt = magic_enum::enum_cast<ShaderParamType>(static_cast<std::string_view>(paramJson["Type"].get<std::string>()));

					// 失敗したら次へ
					if (!opt) continue;

					// 成功したらパラメータに追加
					param.type = opt.value();

					// 値を取得
					param.value = MaterialConstant::GetVariant(ctx, param.type, paramJson["Value"]);

					// リストに追加
					paramsList.insert({ paramJson["Name"], param });
				}

				material.m_pendingParams[type] = paramsList;
			}

			// 閉じる
			ifs.close();

			// 返す
			result.asset = std::make_unique<MaterialAsset>(material);

			return result;
		}
	}
}
