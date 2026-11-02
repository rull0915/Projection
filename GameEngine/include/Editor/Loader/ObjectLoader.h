/**
 * @file    ObjectLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/29
 *
 * @brief   オブジェクトをjsonファイルから読み込むクラス
 */

#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "Common/Property/PropertyObject.h"
#include "Assets/Managers/AssetManager.h"
#include "Common/Property/Property.h"

namespace RHEngine
{
	class GameObject;
	class Scene;
	class ObjectManager;

	/**
	 * @brief オブジェクトをjsonファイルから読み込むクラス
	 */
	class ObjectLoader
	{
	private:

		/** @brief アセットマネージャー (Handleの解決に使用) */
		AssetManager& m_assetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager アセットマネージャー
		 */
		ObjectLoader(AssetManager& assetManager)
			: m_assetManager{ assetManager }
		{
		}

		~ObjectLoader() = default;

		/// @}

		/// @name ロード関数
		/// @{

		/**
		 * @brief プロパティのロード
		 * @param json プロパティ情報を持つjson
		 * @param property 読み込み対象のプロパティ
		 * @param pScene 所属しているシーン
		 */
		void LoadProperty(const nlohmann::json& json, Property& property, Scene* pScene);

		/**
		 * @brief プロパティオブジェクトのロード
		 * @param json プロパティオブジェクト情報を持つjson
		 * @param obj 読み込み対象のオブジェクト
		 * @param pScene 所属しているシーン
		 */
		void LoadPropertyObject(const nlohmann::json& json, PropertyObject& obj, Scene* pScene);

		/**
		 * @brief GameObjectのロード
		 * @param json ゲームオブジェクト情報を持つjson
		 * @param pObj 読み込み対象のオブジェクト
		 * @param pScene 所属しているシーン
		 */
		void LoadObject(const nlohmann::json& json, GameObject* pObj, Scene* pScene);

		/**
		 * @brief オブジェクトマネージャーのロード
		 * @param json 
		 * @param pScene 
		 */
		void LoadObjectManager(const nlohmann::json& json, Scene* pScene);

		/**
		 * @brief シーンのロード
		 * @param json シーン情報を持つjson
		 * @param pScene 読み込み対象のシーン
		 */
		void LoadScene(const nlohmann::json& json, Scene* pScene);

		/// @}
		
		/// @name ファイルから直接読み込む関数
		/// @{

		/**
		 * @brief プロパティオブジェクトをファイルから直接読み込む
		 * @param filePath jsonファイルのパス
		 * @param pObj 読み込み対象のプロパティオブジェクト
		 */
		void LoadPropertyFromFile(const std::wstring& filePath, PropertyObject* pObj);

		/**
		 * @brief ゲームオブジェクトをファイルから直接読み込む
		 * @param filePath jsonファイルのパス
		 * @param pObj 読み込み対象のゲームオブジェクト
		 */
		void LoadObjectFromFile(const std::wstring& filePath, GameObject* pObj);

		/**
		 * @brief シーンをファイルから直接読み込む
		 * @param filePath jsonファイルのパス
		 * @param pScene 読み込み対象のシーン
		 */
		void LoadSceneFromFile(const std::wstring& filePath, Scene* pScene);

		/// @}
	};
} // namespace RHEngine
