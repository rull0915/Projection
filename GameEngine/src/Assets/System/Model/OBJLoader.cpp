//====================================================//
// ファイル名  : OBJLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/07
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#include <fstream>

#include "OBJLoader.h"
#include "Assets/Types/Vertex/VertexTypes.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Loader
	{
		std::unique_ptr<Model> OBJLoader(const std::filesystem::path& path)
		{
			// ------ 拡張子がobjでなければスキップ ------ //

			// 拡張子取得
			std::string ext = path.extension().string();

			// 小文字に変換する
			std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
				return std::tolower(c);
				});

			// objか調べる
			if (ext != "obj") return nullptr;

			// 1行ずつ読み込む

			// ------ ファイル読み込み ------ //

			// ifsに変換
			std::ifstream ifs(path);

			// 開けなかったら生成しない
			if (!ifs.is_open())
			{
				ifs.close();

				return nullptr;
			}

			// 1行ずつ格納する用の文字列
			std::string line{};

			// 各情報の一時キャッシュ
			std::vector<DirectX::XMFLOAT3> positions{};		// 頂点座標	
			std::vector<DirectX::XMFLOAT2> texcoords{};		// uv座標
			std::vector<DirectX::XMFLOAT3> normals{};		// 法線ベクトル

			std::vector<VertexPositionNormalTexture> vertices;	// 頂点
			std::unordered_map<VertexKey, uint32_t> vertexkeys;	// 頂点インデックス
			std::vector<uint32_t> indices;					// インデックス

			// 1行ずつ調べる
			while (std::getline(ifs, line))
			{
				// sstreamに変換
				std::istringstream stream(line);

				// スペース区切りで格納するための変数
				std::string type;

				// スペース区切り
				stream >> type;

				// typeによって分岐
				if (type == "v")		// 頂点座標
				{
					DirectX::XMFLOAT3 p;
					stream >> p.x >> p.y >> p.z;
					positions.push_back(p);
				}
				else if (type == "vt")	// uv座標
				{
					DirectX::XMFLOAT2 t;
					stream >> t.x >> t.y;
					texcoords.push_back(t);
				}
				else if (type == "vn")	// 法線ベクトル
				{
					DirectX::XMFLOAT3 n;
					stream >> n.x >> n.y >> n.z;
					normals.push_back(n);
				}
				else if (type == "f")	// 面情報
				{
					// 面に含まれる頂点情報の一覧
					std::vector<VertexKey> faceVertices{};

					std::string vertex;

					// 全ての頂点をループ
					while (stream >> vertex)
					{
						// 何番目の要素か
						int index = 0;

						// sstreamに変換
						std::istringstream vstream(vertex);

						// スラッシュ区切りで格納する用の変数
						std::string value{};

						// 作成するVertexKey
						VertexKey key{ 0, 0, 0 };

						// スラッシュ区切りで分割
						while (std::getline(vstream, value, '/'))
						{
							// 空文字列じゃない場合
							if (!value.empty())
							{
								switch (index)
								{
								case 0:
									// v
									key.position = std::stoi(value);
									break;

								case 1:
									// vt
									key.texcoord = std::stoi(value);
									break;

								case 2:
									// vn
									key.normal = std::stoi(value);
									break;
								}
							}

							// インデックスを加算
							++index;
						}

						// 配列に追加
						faceVertices.push_back(key);
					}

					// 頂点キャッシュの追加
					for (auto& key : faceVertices)
					{
						// 検索
						auto it = vertexkeys.find(key);

						// あれば次へ
						if (it != vertexkeys.end()) continue;

						// なければ追加
						vertices.emplace_back
						(
							GetOBJElement(positions, key.position), 
							GetOBJElement(normals, key.normal), 
							GetOBJElement(texcoords, key.texcoord)
						);

						vertexkeys[key] = vertices.size() - 1;
					}

					// インデックスキャッシュの追加
					for (size_t i = 0; i < faceVertices.size() - 2; ++i)
					{
						// TriangleFan形式を採用
						// Todo: 汎用性は低いためEarCliping法などに改善すると良い
						indices.push_back(vertexkeys[faceVertices[0]]);
						indices.push_back(vertexkeys[faceVertices[i + 1]]);
						indices.push_back(vertexkeys[faceVertices[i + 2]]);
					}
				}
				else if (type == "usemtl")
				{

				}
				else if (type == "mtllib")
				{

				}
				else if (type == "o")
				{

				}
			}
		}
	}
}
