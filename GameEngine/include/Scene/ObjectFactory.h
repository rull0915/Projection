/**
 * @file    ObjectFactory.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/12
 *
 * @brief   オブジェクトの生成を担うクラス
 */

#pragma once

#include <SimpleMath.h>

#include "GameObject/GameObject.h"
#include "Assets/Types/Prefab.h"
#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	class Scene;
	class AssetManager;

	/**
	 * @brief オブジェクトのファクトリ
	 */
	class ObjectFactory
	{
	private:

		/** @brief シーンポインタ */
		Scene* m_pScene;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene シーンポインタ
		 * @param assetManager アセットマネージャー
		 */
		ObjectFactory(Scene* pScene, AssetManager& assetManager);

		~ObjectFactory() = default;

		/// @}

		/** @brief オブジェクトを生成する関数 */
		GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });

		/** @brief プレハブからオブジェクトを生成する関数 */
		GameObject* Instantiate(Prefab* prefab) const;

		/** @brief プレハブハンドルからオブジェクトを生成する関数 */
		GameObject* Instantiate(Handle<Prefab> prefab) const;

		/** @brief UIオブジェクトを生成する関数 */
		GameObject* GenerateUI();
	};
}	// namespace RHEngine
