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
#include "Assets/Managers/AssetLoadContext.h"
#include "Assets/Types/Vertex/VertexTypes.h"
#include "System/GraphicsManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Loader
	{
		std::unique_ptr<Model> OBJLoader::Load(const std::filesystem::path& path, AssetLoadContext& ctx)
		{
			// ------- 拡張子がobjでなければスキップ ------- //

			// 拡張子取得
			std::string ext = path.extension().string();

			// 小文字に変換する
			std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
				return std::tolower(c);
				});

			// objか調べる
			if (ext != ".obj") return nullptr;

			// 1行ずつ読み込む

			// -------------- ファイル読み込み ------------- //

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
			std::unordered_map<VertexKey, uint32_t, VertexKeyHash> vertexkeys;	// 頂点インデックス
			std::vector<uint32_t> indices;					// インデックス

			// 作られたメッシュの一覧
			std::vector<std::pair<std::string, Mesh>> meshs;

			// 作られたサブメッシュの一覧
			std::vector<SubMesh> subMeshs;

			// 現在作成中のメッシュ名
			std::string currentMeshName = "Mesh0";

			// サブメッシュを1つ確定するラムダ式
			auto confirmSubMesh =
				[&subMeshs, &indices]()
				{
					// インデックスがなければ登録しない
					if (indices.empty()) return;

					// サブメッシュを確定
					SubMesh subMesh{};

					// 1つ前のサブメッシュを調べる
					if (!subMeshs.empty())
					{
						SubMesh& last = subMeshs.back();

						subMesh.indexOffset = last.indexOffset + last.indexCount;	// 前のサブメッシュの管理下の次から使用
						subMesh.indexCount = static_cast<UINT>(indices.size()) - subMesh.indexOffset;	// 前のサブメッシュからここまでに作られた全てを使用
						subMesh.vertexOffset = 0;	// 加算なし
					}
					// 最初のサブメッシュの場合
					else
					{
						subMesh.indexOffset = 0;	// 0番から使用
						subMesh.indexCount = static_cast<UINT>(indices.size());	// これまでに記録された全てのインデックスを使用
						subMesh.vertexOffset = 0;	// 加算なし
					}

					// 配列に追加
					subMeshs.push_back(subMesh);
				};

			// メッシュを一つ確定するラムダ式
			auto confirmMesh =
				[&]()
				{
					// サブメッシュがなければ登録しない
					if (subMeshs.empty()) return;

					Mesh mesh{};

					// デバイスを取得
					auto device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

					// 作成された頂点リストからバッファを作成
					D3D11_BUFFER_DESC desc = {};
					desc.Usage = D3D11_USAGE_DEFAULT;
					desc.ByteWidth = sizeof(VertexPositionNormalTexture) * static_cast<UINT>(vertices.size());    // バッファのサイズ
					desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // 頂点バッファとして使用
					desc.CPUAccessFlags = 0;                        // CPU からアクセスは不要
					desc.MiscFlags = 0;
					desc.StructureByteStride = 0;

					D3D11_SUBRESOURCE_DATA initData = {};
					initData.pSysMem = vertices.data(); // CPU 側のデータのポインタを渡す
					initData.SysMemPitch = 0;           // 頂点バッファの場合、ピッチは不要（ 0 にする）
					initData.SysMemSlicePitch = 0;      // 頂点バッファの場合、スライスも不要（ 0 にする）

					DX::ThrowIfFailed(
						device->CreateBuffer(&desc, &initData, mesh.m_vertexBuffer.ReleaseAndGetAddressOf())
					);

					// インデックスバッファの作成 頂点バッファで使ったものを上書して再利用
					desc.ByteWidth = sizeof(uint32_t) * static_cast<UINT>(indices.size());    // バッファのサイズ
					desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      // インデックスバッファとして使用
					
					initData.pSysMem = indices.data(); // CPU 側のデータのポインタを渡す

					DX::ThrowIfFailed(
						device->CreateBuffer(&desc, &initData, mesh.m_indexBuffer.ReleaseAndGetAddressOf())
					);

					// サブメッシュを渡す
					mesh.m_subMeshes = subMeshs;

					// 各キャッシュのリセット
					vertices.clear();
					indices.clear();
					vertexkeys.clear();
					subMeshs.clear();

					// 配列に追加
					meshs.push_back({ currentMeshName, mesh });
				};

			// ---------- 1行ずつ調べる ---------- //
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

						vertexkeys[key] = static_cast<uint32_t>(vertices.size() - 1);
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
					// ここまでのサブメッシュを確定
					confirmSubMesh();
				}
				else if (type == "mtllib")
				{
					// マテリアルファイルの読み込み
				}
				else if (type == "o")
				{
					// サブメッシュを確定
					confirmSubMesh();

					// メッシュを確定
					confirmMesh();

					// 名前を更新
					std::string newName{};
					stream >> newName;

					// 名前が空文字ならインデックスで確定する
					currentMeshName = newName.empty() ? "Mesh" + std::to_string(meshs.size()) : newName;
				}
			}

			// 最後のサブメッシュ・メッシュを確定
			confirmSubMesh();
			confirmMesh();

			// ----------- 作られたメッシュを登録する ----------- //

			// 新しいサブアセットがあるかどうかのフラグ
			bool existNewSubAsset = false;

			auto& db = ctx.GetDataBase();

			// Auxを取得
			AssetAux aux = db.GetAux(db.GetUUID(path));

			for (auto& mesh : meshs)
			{
				// 仮想パスを生成
				std::filesystem::path virtualPath = path.string() + "#" + mesh.first;

				// UUIDを取得
				UUID uuid = db.GetUUID(virtualPath);

				// 未登録なら
				if (uuid == UUID_NONE)
				{
					// 新規サブアセットフラグを立てる
					existNewSubAsset = true;

					// UUIDを生成
					uuid = db.GenerateUUID();

					// サブアセット情報
					SubAssetInfo info;

					info.assetType = "Mesh";
					info.uuid = uuid;
					info.name = mesh.first;

					aux.subAssets.push_back(info);
				}

				// 登録
				auto handle = ctx.GetRegistry().Register(uuid);
				ctx.GetRegistry().Replace(handle.index, std::make_unique<Mesh>(mesh.second));
			}

			// 変更があれば.auxファイルの更新
			if (existNewSubAsset)
			{
				db.SaveAux(aux, path);
			}

			// 返す
			return std::make_unique<Model>();
		}
	}
}
