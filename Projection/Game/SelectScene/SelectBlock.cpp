//====================================================//
// ファイル名  : SelectBlock.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "SelectBlock.h"

#include "Physics/HitContact.h"
#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"
#include "Input/InputSystem.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SelectBlock::SelectBlock(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_targetScene{}
	, m_targetDialog{}
{
	ADD_PROPERTY(SelectBlock, m_targetScene);
	ADD_PROPERTY(SelectBlock, m_targetDialog);
	ADD_PROPERTY(SelectBlock, m_nameText);
}

void SelectBlock::Start()
{
}

void SelectBlock::OnTriggerStay(RHEngine::HitContact & contact)
{
	// プレイヤーが当たっているとき
	if (contact.other->GetTag() == "Player")
	{
		// スペースキーでシーン移動
		if (RHEngine::Input::Custom::GetButtonDown("Decide"))
		{
			RHEngine::SceneManager::Instance().RequestSceneChange(m_targetScene,
				std::make_unique<RHEngine::Transition::Fade>(),
				std::make_unique<RHEngine::Transition::Fade>()
				);
		}

		// テキストを変更
		if (m_nameText) m_nameText->SetText(m_targetScene);
	}
}

void SelectBlock::OnTriggerEnter(RHEngine::HitContact& contact)
{
	contact;

	// UIを表示
	if (m_targetDialog) m_targetDialog->Show();
}

void SelectBlock::OnTriggerExit(RHEngine::HitContact & contact)
{
	contact;

	// UIを非表示
	if (m_targetDialog) m_targetDialog->Hide();
}
