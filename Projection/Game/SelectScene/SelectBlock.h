//====================================================//
// ファイル名   : SelectBlock.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : 選択
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(SelectBlock)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameLib/UI/Dialog.h"
#include "Components/UI/Graphics/TextUI.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class SelectBlock : public RHEngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ターゲットシーン名
	std::string m_targetScene;

	// ターゲットダイアログ
	RHEngine::Ref<Dialog> m_targetDialog;

	// ステージ名テキスト
	RHEngine::Ref<RHEngine::TextUI> m_nameText;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	SelectBlock(RHEngine::IComponentOwner* own);
	~SelectBlock() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------

	COMPONENT_TYPE(SelectBlock, RHEngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void OnTriggerStay(RHEngine::HitContact& contact) override;
	void OnTriggerEnter(RHEngine::HitContact& contact) override;
	void OnTriggerExit(RHEngine::HitContact& contact) override;
};
