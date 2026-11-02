/**
 * @file    InspectorWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 *
 * @brief   インスペクターウィンドウ
 */

#pragma once

#ifdef ENGINE_GUI

#include "PropertyOnInspector.h"
#include "Common/Property/PropertyObject.h"
#include "GameObject/GameObject.h"
#include "Assets/Objects/AssetBase.h"

#include "Components/ComponentInfo.h"
#include "Editor/Editor/SelectedOnGUI.h"

namespace RHEngine
{
	class AssetManager;
	class Scene;

	/**
	 * @brief インスペクターウィンドウ
	 */
	class InspectorWindow
	{
	private:
		/** @brief プロパティの表示担当 */
		PropertyOnInspector m_propertyOnInspector;

		/** @brief 選択中のオブジェクト */
		SelectedOnGUI& m_selected;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 編集対象のシーンポインタ
		 * @param assetManager アセットマネージャー
		 * @parassetManager selected 選択オブジェクト
		 */
		InspectorWindow(Scene* pScene, AssetManager& assetManager, SelectedOnGUI& selected)
			: m_propertyOnInspector(pScene, assetManager)
			, m_selected{ selected }
			, m_assetManager{ assetManager }
		{};

		~InspectorWindow() = default;

		/// @}

		/**
		 * @brief インスペクターウィンドウを表示する
		 */
		void DrawInspector();

	private:

		/**
		 * @brief インスペクターウィンドウへの描画を開始する
		 */
		void StartInspector();

		/**
		 * @brief 1つのPropertyObjectをインスペクターウィンドウに表示する
		 * @param pObject 対象のObject
		 */
		void DrawPropertyObjectOnInspector(PropertyObject* pObject);

		/**
		 * @brief 1つのGameObjectをインスペクターウィンドウに表示する
		 * @param pObject 対象のObject
		 */
		void DrawGameObject(GameObject* pObject);

		/**
		 * @brief コンポーネント追加のメニューを表示する
		 * @param pObject 追加対象のGameObject
		 */
		void DrawAddComponent(GameObject* pObject);

		/**
		 * @brief コンポーネントをリストに表示する
		 * @param pObject 所有者
		 * @param info 表示するコンポーネントの情報
		 */
		void DrawComponentList(GameObject* pObject, ComponentInfo info);

		/**
		 * @brief Assetをインスペクターウィンドウに表示する
		 * @param pAsset 対象のアセット
		 */
		void DrawAsset(AssetBase* pAsset);
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
