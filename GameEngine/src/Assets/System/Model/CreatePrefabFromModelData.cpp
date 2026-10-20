//====================================================//
// ファイル名  : CreatePrefabFromModelData.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CreatePrefabFromModelData.h"

#include "Assets/System/Prefab/PrefabBuilder.h"

#include "Components/World/Mesh/MeshFilter.h"
#include "Components/World/Renderer/MeshRenderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	std::unique_ptr<Prefab> CreatePrefabFromModelData::Create(const std::string& name, const IAssetResolver& resolver, const std::unordered_map<std::string, std::pair<Handle<Mesh>, Mesh>>& meshes, const std::unordered_map<std::string, Handle<MaterialAsset>>& materials)
	{
		// 空の一時オブジェクトを作成
		std::unique_ptr<GameObject> gameObject = PrefabBuilder::CreateDefaultObject();

		// 名前を設定
		gameObject->SetName(name);

		// 関数実行中に子オブジェクトを保持し続けるための変数
		std::vector<std::unique_ptr<GameObject>> children;

		// トランスフォームを追加
		Transform* parentTransform = gameObject->AddComponent<Transform>();

		// 全メッシュをループ
		for (auto& [name, meshInfo] : meshes)
		{
			// 子オブジェクトを生成
			std::unique_ptr<GameObject> child = PrefabBuilder::CreateDefaultObject();

			// 名前を設定
			child->SetName(name);

			// トランスフォームを追加
			Transform* childTransform = child->AddComponent<Transform>();

			// 親を設定
			childTransform->SetParent(parentTransform);

			// メッシュを設定
			MeshFilter* meshFilter = child->AddComponent<MeshFilter>();
			meshFilter->SetMesh(meshInfo.first);

			// メッシュレンダラーを追加
			MeshRenderer* meshRenderer = child->AddComponent<MeshRenderer>();

			// マテリアル一覧を取得
			auto& meshMaterials = meshRenderer->GetMaterials();

			// リセット
			meshMaterials.clear();

			// サブメッシュを全て調べる
			for (auto& subMesh : meshInfo.second.GetSubMeshes())
			{
				// 対応するマテリアルを取得
				Handle<MaterialAsset> material = materials.at(subMesh.materialName);

				// マテリアルを追加
				meshMaterials.push_back(material);
			}

			// 所有権を移動
			children.push_back(std::move(child));
		}

		// Prefabを生成
		return PrefabBuilder::SaveToPrefab(gameObject.get(), resolver);
	}
}
