//====================================================//
// ファイル名   : PrefabBuilder.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/13
//
// 概要 : プレハブで使用する一時的なGameObjectを作成するクラス
//
// 更新履歴 :
// 2026/10/13 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>

#include "Assets/Types/Prefab.h"
#include "Assets/Types/PrefabImpl.h"
#include "GameObject/GameObject.h"

#include "Editor/Loader/ObjectLoader.h"
#include "Editor/Saver/ObjectSaver.h"
#include "Assets/Managers/AssetManager.h"
#include "Assets/Managers/IAssetResolver.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class PrefabBuilder
	{
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
	public:
		// PrefabからSceneに属さない一時GameObjectを構築する関数
		static std::unique_ptr<GameObject> CreateTempObject(Prefab* prefab, AssetManager* assetManager)
		{
			// オブジェクトの生成
			auto pObj = std::make_unique<GameObject>(GameObject::CreateToken{});

			// UUIDを新規作成しセット
			pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

			// ローダーを作成
			ObjectLoader loader(*assetManager);

			// Temp用のコンテキストでロード
			loader.LoadObject(prefab->GetImpl()->GetJson(), pObj.get(), nullptr);

			return pObj;
		}

		// 空の一時GameObjectを作成する関数
		static std::unique_ptr<GameObject> CreateDefaultObject()
		{
			// オブジェクトの生成
			auto pObj = std::make_unique<GameObject>(GameObject::CreateToken{});

			// UUIDを新規作成しセット
			pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

			return pObj;
		}

		// 一時GameObjectからPrefabAssetを生成する関数
		static std::unique_ptr<Prefab> SaveToPrefab(GameObject* pTempObj, const IAssetResolver& assetManager)
		{
			// セーバーを作成
			ObjectSaver saver(assetManager);

			// 保存
			nlohmann::json json = saver.SaveObject(pTempObj);

			// プレハブを生成
			return std::make_unique<Prefab>(std::make_unique<PrefabImpl>(json));
		}
	};
}
