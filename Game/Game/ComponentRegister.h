//====================================================//
// ファイル名   : ComponentRegister.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要 : コンポーネントの登録をするクラス
//
// 更新履歴 :
// 2026/07/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Editor/Loader/ComponentFactory.h"
#include "GameObject/GameObject.h"

// 生成関数登録マクロ
#define REGIST_TO_FACTORY(Name) (REngine::ComponentFactory::Register(#Name, { REngine::ComponentProject::Game, Name::SPACE }, [](REngine::GameObject* o){ return o->AddComponent<Name>(); }))

//====================================================//
// クラス宣言
//====================================================//
class ComponentRegister
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ComponentRegister() = default;
	~ComponentRegister() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
	static void RegistComponents();
};
