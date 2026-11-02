/**
 * @file    CreatePrefabFromModelData.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 *
 * @brief   モデルデータからプレハブを作成する関数
 */

#pragma once

#include <string>
#include <utility>
#include <memory >
#include <unordered_map>

#include "Assets/Types/Prefab.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Model/Mesh.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/IAssetResolver.h"

namespace RHEngine
{
	/**
	 * @brief モデルデータからプレハブを作成する
	 */
	class CreatePrefabFromModelData
	{
	public:
		
		/**
		 * @brief プレハブを作成する
		 * @param name プレハブ名
		 * @param resolver アセットリゾルバ (Prefabに保存するために使用)
		 * @param meshes モデルが持つメッシュデータのリスト
		 * @param materials モデルが持つマテリアルデータのリスト
		 * @return 作成したプレハブ
		 */
		static std::unique_ptr<Prefab> Create(
			const std::string& name, 
			const IAssetResolver& resolver, 
			const std::unordered_map<std::string, std::pair<Handle<Mesh>, Mesh>>& meshes, 
			const std::unordered_map<std::string, Handle<MaterialAsset>>& materials
		);
	};
}
