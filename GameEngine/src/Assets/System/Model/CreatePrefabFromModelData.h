//====================================================//
// ファイル名   : CreatePrefabFromModelData.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要 : モデルデータからプレハブを作成する関数
//
// 更新履歴 :
// 2026/10/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <string>
#include <utility>
#include <unordered_map>

#include "Assets/Types/Prefab.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Model/Mesh.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/IAssetResolver.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class CreatePrefabFromModelData
	{
	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		static std::unique_ptr<Prefab> Create(const std::string& name, const IAssetResolver& resolver, const std::unordered_map<std::string, std::pair<Handle<Mesh>, Mesh>>& meshes, const std::unordered_map<std::string, Handle<MaterialAsset>>& materials);
	};
}
