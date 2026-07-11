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
#include "Common/PropertyObject.h"
#include "GameObject/GameObject.h"

#include "Components/ComponentCategory.h"

//====================================================//
// クラス宣言
//====================================================//
class InspectorWindow
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	InspectorWindow() = default;
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

	// GameObjectの情報をInspectorに表示する関数
	void DrawPropertyObjectOnInspector(PropertyObject* object);

	// プロパティを一つ表示する関数
	bool DrawProperty(const Property* property);

	// コンポーネントの追加を表示する関数
	void DrawAddComponent(GameObject* object);

	// コンポーネントリストを表示する関数
	void DrawComponentList(GameObject* object, ComponentInfo info);
};
