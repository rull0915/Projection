//====================================================//
// ファイル名  : MtlFileLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/13
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "MtlFileLoader.h"

#include <fstream>
#include "Assets/EngineDefaultAssets.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	std::unordered_map<std::string, MaterialAsset> RHEngine::MtlFileLoader::LoadMTL(const std::filesystem::path& path, AssetContext& ctx)
	{
		// ------- 拡張子がmtlでなければスキップ ------- //

			// 拡張子取得
		std::string ext = path.extension().string();

		// 小文字に変換する
		std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
			return std::tolower(c);
			});

		// mtlか調べる
		if (ext != ".mtl")
		{
			return {};
		}

		// -------------- ファイル読み込み ------------- //

		// ifsに変換
		std::ifstream ifs(path);

		// 開けなかったら生成しない
		if (!ifs.is_open())
		{
			ifs.close();

			return{};
		}

		// 作成したマテリアルを保持しておく配列
		std::unordered_map<std::string, MaterialAsset> materials{};

		// 作成中のマテリアル
		std::unordered_map<std::string, MaterialAsset::Parameter> currentParams{};

		// 作成中のマテリアル名
		std::string currentMaterialName{};

		// ---------- 使用するシェーダーアセットを読み込む ---------- //

		Handle<ShaderAsset> ps = ctx.LoadFromUUID(RHEngine::DefaultAsset::LIT_PS).As<ShaderAsset>();
		Handle<ShaderAsset> vs = ctx.LoadFromUUID(RHEngine::DefaultAsset::LIT_VS).As<ShaderAsset>();

		// 作成中のマテリアルをリストに追加するラムダ式
		auto createMaterial =
			[&]() {
			// マテリアルを作成
			MaterialAsset material{};

			material.SetName(currentMaterialName);

			// デフォルト値を指定
			currentParams.insert({ "DiffuseIntensity", { ShaderParamType::Float, 1.0f } });
			currentParams.insert({ "SpecularIntensity", { ShaderParamType::Float, 1.0f } });

			// ピクセルシェーダーのロード用パラメータをセット
			material.SetPendingParams(ShaderType::Pixel, currentParams);

			// シェーダーをセット
			material.SetShader(vs, ShaderType::Vertex);
			material.SetShader(ps, ShaderType::Pixel);

			// リストに追加
			materials.insert({ currentMaterialName, material });
			};

		// ---------- パラメータを構築するラムダ式 ---------- //

		// sstreamからfloatを取得する関数
		auto getFloatParam = [](std::istringstream& ss)
			{
				// sstreamから情報を取得
				float value{};
				ss >> value;

				// パラメータを構築して返す
				return MaterialAsset::Parameter(ShaderParamType::Float, value);
			};

		// sstreamからカラーを取得する関数
		auto getColorParam = [](std::istringstream& ss)
			{
				// sstreamから色情報を取得
				DirectX::SimpleMath::Color color{};
				ss >> color.x >> color.y >> color.z;

				// パラメータを構築して返す
				return MaterialAsset::Parameter(ShaderParamType::Color, color);
			};

		// sstreamからテクスチャハンドルパラメータを構築する関数
		auto getTextureParam = [&ctx, &path](std::istringstream& ss)
			{
				// sstreamからファイル名情報を取得
				std::string fileName{};
				ss >> fileName;

				// パスを構築
				std::filesystem::path texPath = path.parent_path() / fileName;

				// ロードする
				UnTypeHandle handle = ctx.LoadFromUUID(ctx.GetDataBase().GetUUID(texPath));

				// パラメータを生成
				return MaterialAsset::Parameter(ShaderParamType::Texture2D, handle.As<Texture>());
			};

		// ---------- 1行ずつ調べる ---------- //
		std::string line{};

		while (std::getline(ifs, line))
		{
			// sstreamに変換
			std::istringstream stream(line);

			// スペース区切りで格納するための変数
			std::string type;

			// スペース区切り
			stream >> type;

			// typeによって分岐
			if (type == "newmtl")	// 新しいマテリアルの場合
			{
				// 既存のマテリアルを保存
				if (!currentMaterialName.empty())
				{
					createMaterial();
				}

				// 名前を取得
				std::string name{};
				stream >> name;

				// 次のマテリアルへ
				currentMaterialName = name;
				currentParams.clear();
			}

			else if (type == "Kd")
			{
				currentParams.insert({ "BaseColor", getColorParam(stream) });
			}

			else if (type == "Ka")
			{
				currentParams.insert({ "AmbientColor", getColorParam(stream) });
			}

			else if (type == "Ks")
			{
				currentParams.insert({ "SpecularColor", getColorParam(stream) });
			}

			else if (type == "Ke")
			{
				currentParams.insert({ "EmissiveColor", getColorParam(stream) });
			}

			else if (type == "map_Kd")
			{
				currentParams.insert({ "BaseColorMap", getTextureParam(stream) });
			}

			else if (type == "Ns")
			{
				currentParams.insert({ "Shininess", getFloatParam(stream) });
			}
		}

		// 最後の1つを確定
		if (!currentMaterialName.empty())
		{
			createMaterial();
		}

		return materials;
	}
}
