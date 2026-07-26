//====================================================//
// ファイル名   : InspectorWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要 : 
//
// 更新履歴 :
// 2026/07/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>

#include "PropertyOnInspector.h"
#include "Common/Property/PropertyObject.h"
#include "GameObject/GameObject.h"

#include "Components/ComponentCategory.h"

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

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		InspectorWindow()
			: m_propertyOnInspector{}
		{};
		~InspectorWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// インスペクターの描画
		bool DrawInspector(PropertyObject* selected);

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
	};
}	// namespace REngine
