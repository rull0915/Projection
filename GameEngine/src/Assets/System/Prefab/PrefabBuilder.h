/**
 * @file    PrefabBuilder.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/13
 *
 * @brief   プレハブで使用する一時的なGameObjectを作成するクラス
 */

#pragma once

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
	/**
	 * @brief プレハブで使用する一時的なオブジェクトを作成する
	 */
	class PrefabBuilder
	{
	public:
		// PrefabからSceneに属さない一時GameObjectを構築する関数

		/**
		 * @brief Prefabからシーンに属さない一時的なオブジェクトを構築する
		 * @param pPrefab 構築対象のプレハブ
		 * @param assetManager アセットマネージャー
		 * @return 生成したオブジェクト
		 */
		static std::unique_ptr<GameObject> CreateTempObject(Prefab* pPrefab, AssetManager& assetManager)
		{
			// オブジェクトの生成
			auto pObj = std::make_unique<GameObject>(GameObject::CreateToken{});

			// UUIDを新規作成しセット
			pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

			// ローダーを作成
			ObjectLoader loader(assetManager);

			// Temp用のコンテキストでロード
			loader.LoadObject(pPrefab->GetImpl()->GetJson(), pObj.get(), nullptr);

			return pObj;
		}

		/**
		 * @brief シーンに所属しない空のゲームオブジェクトを作成する
		 * @return 作成したオブジェクト
		 */
		static std::unique_ptr<GameObject> CreateDefaultObject()
		{
			// オブジェクトの生成
			auto pObj = std::make_unique<GameObject>(GameObject::CreateToken{});

			// UUIDを新規作成しセット
			pObj->SetUUID(UUIDRegistry::Instance().GenerateUUID());

			return pObj;
		}

		/**
		 * @brief ゲームオブジェクトからプレハブを生成する
		 * @param pTempObj オブジェクト
		 * @param assetManager アセットマネージャー
		 * @return 生成したプレハブ
		 */
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
}	// namespace RHEngine
