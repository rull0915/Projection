/**
 * @file    ObjectSaver.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/29
 *
 * @brief   コンポーネントをjsonファイルに保存するクラス
 */

#pragma once

#include <filesystem>

#include "ThirdParty/nlohmann/json.hpp"
#include "Assets/Managers/IAssetResolver.h"
#include "Common/Property/Property.h"
#include "Common/Property/PropertyObject.h"

namespace RHEngine
{
	class GameObject;
	class ObjectManager;
	class Scene;

	/**
	 * @brief オブジェクトをjsonへ保存するクラス
	 */
	class ObjectSaver
	{
	private:
		/** @brief アセットのUUIDを解決する */
		const IAssetResolver& m_assetResolver;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetResolver アセットリゾルバー
		 */
		ObjectSaver(const IAssetResolver& assetResolver)
			: m_assetResolver{ assetResolver }
		{
		}

		~ObjectSaver() = default;

		/// @}

		/// @name 保存関数
		/// @{

		/**
		 * @brief プロパティの保存
		 * @param obj 保存対象のプロパティ
		 * @return 保存したjson
		 */
		nlohmann::json SaveProperty(Property& obj);

		/**
		 * @brief プロパティオブジェクトの保存
		 * @param obj 保存対象のプロパティオブジェクト
		 * @return 保存したjson
		 */
		nlohmann::json SavePropertyObject(PropertyObject& obj);

		/**
		 * @brief ゲームオブジェクトの保存
		 * @param pObj 保存対象のゲームオブジェクト
		 * @return 保存したjson
		 */
		nlohmann::json SaveObject(GameObject* pObj);

		/**
		 * @brief オブジェクトマネージャーの保存
		 * @param pObjectManager 保存対象のマネージャー
		 * @return 保存したjson
		 */
		nlohmann::json SaveObjectManager(const ObjectManager* pObjectManager);

		/**
		 * @brief シーンの保存
		 * @param pScene 保存対象のシーン
		 * @return 保存したjson
		 */
		nlohmann::json SaveScene(const Scene* pScene);

		/// @}

		/// @name ファイルへ直接保存する関数
		/// @{

		/**
		 * @brief ゲームオブジェクトをファイルへ直接保存する
		 * @param filePath 保存先のjsonファイルパス
		 * @param pObj 保存対象のゲームオブジェクト
		 */
		void SaveObjectToFile(const std::filesystem::path& filePath, GameObject* pObj);

		/**
		 * @brief シーンをファイルへ直接保存する
		 * @param filePath 保存先のjsonファイルパス
		 * @param scene 保存対象のシーン
		 */
		void SaveSceneToFile(const std::filesystem::path& filePath, Scene* scene);

		/**
		 * @brief プロパティオブジェクトをファイルへ直接保存する
		 * @param filePath 保存先のjsonファイルパス
		 * @param pObj 保存対象のプロパティオブジェクト
		 */
		void SavePropertyToFile(const std::filesystem::path& filePath, PropertyObject* pObj);

		/// @}
	};
} // namespace RHEngine
