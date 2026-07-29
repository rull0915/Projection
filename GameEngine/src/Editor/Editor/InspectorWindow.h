//====================================================//
// ファイル名   : InspectorWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要 : インスペクターウィンドウ
//
// 更新履歴 :
// 2026/07/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "PropertyOnInspector.h"
#include "Common/Property/PropertyObject.h"
#include "GameObject/GameObject.h"
#include "Assets/Objects/AssetBase.h"

#include "Components/ComponentCategory.h"
#include "Editor/Editor/SelectedOnGUI.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class InspectorWindow
	{
	private:
		// プロパティの表示を担当するクラス
		PropertyOnInspector m_propertyOnInspector;

		// 選択中オブジェクト
		SelectedOnGUI& m_selected;

		// アセットマネージャー
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		InspectorWindow(AssetManager& am, SelectedOnGUI& selected)
			: m_propertyOnInspector(am)
			, m_selected{ selected }
			, m_assetManager{ am }
		{};
		~InspectorWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// インスペクターの描画
		bool DrawInspector();

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// インスペクター描画の開始
		bool StartInspector();

		// PropertyObjectの情報をInspectorに表示する関数
		void DrawPropertyObjectOnInspector(PropertyObject* object);

		// GameObjectを表示する関数
		void DrawGameObject(GameObject* object);

		// コンポーネントの追加を表示する関数
		void DrawAddComponent(GameObject* object);

		// コンポーネントリストを表示する関数
		void DrawComponentList(GameObject* object, ComponentInfo info);

		// Assetを表示する関数
		void DrawAsset(AssetBase* asset);
	};
}	// namespace REngine
