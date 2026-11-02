/**
 * @file    PropertyOnInspector.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/17
 *
 * @brief   プロパティをインスペクターに表示するクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <SimpleMath.h>

#include "Common/Property/PropertyObject.h"
#include "Common/Property/Property.h"

namespace RHEngine
{
	class AssetManager;
	class Scene;

	/**
	 * @brief プロパティをインスペクターウィンドウに表示する
	 */
	class PropertyOnInspector
	{
	private:

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/** @brief 編集中クォータニオンのキャッシュ */
		DirectX::SimpleMath::Vector3 m_quaternionCache;

		/** @brief クォータニオン編集中フラグ */
		bool m_quaternionEditing;		

		/** @brief シーンポインタ */	
		Scene* m_pScene;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 編集対象のシーン
		 * @param assetManager アセットマネージャー
		 */
		PropertyOnInspector(Scene* pScene, AssetManager& assetManager)
			: m_assetManager{ assetManager }
			, m_quaternionCache{ DirectX::SimpleMath::Vector3::Zero }
			, m_quaternionEditing{ false }
			, m_pScene{ pScene }
		{
		}

		~PropertyOnInspector() = default;

		/// @}

		/**
		 * @brief プロパティオブジェクトをインスペクターウィンドウに表示する
		 * @param pObject 対象のオブジェクト
		 * @param readOnly 編集不可かどうか
		 * @return 変更されたかどうか
		 */
		bool DrawPropertyObject(PropertyObject* pObject, bool readOnly = false);

		/**
		 * @brief プロパティを1つインスペクターウィンドウに表示する
		 * @param pProperty 対象のプロパティ
		 * @return 変更されたかどうか
		 */
		bool DrawProperty(Property* pProperty);

	private:

		/// @name 各プロパティの表示関数
		/// @{

		/** @brief QuaternionをGUIに表示する */
		bool DrawQuaternion(Property* property);

		/** @brief PropertyObjectをGUIに表示する */
		bool DrawObject(Property* property);
		
		/** @brief 列挙型をGUIに表示する */
		bool DrawEnum(Property* property);
		
		/** @brief AssetHandleをGUIに表示する */
		bool DrawAssetHandle(Property* property);

		/** @brief RefをGUIに表示する */
		bool DrawReference(Property* property);

		/** @brief 配列をGUIに表示する */
		bool DrawArray(Property* property);

		/** @brief 範囲をGUIに表示する */
		bool DrawRange(Property* property);

		/// @}
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
