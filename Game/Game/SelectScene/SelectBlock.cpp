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
SelectBlock::SelectBlock(REngine::IComponentOwner* own)
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

void SelectBlock::OnTriggerStay(REngine::HitContact & contact)
{
	// プレイヤーが当たっているとき
	if (contact.other->GetTag() == "Player")
	{
		// スペースキーでシーン移動
		if (REngine::Input::Custom::GetButtonDown("Decide"))
		{
			REngine::SceneManager::Instance().RequestSceneChange(m_targetScene,
				std::make_unique<REngine::Transition::Fade>(),
				std::make_unique<REngine::Transition::Fade>()
				);
		}

		// テキストを変更
		if (m_nameText) m_nameText->SetText(m_targetScene);
	}
}

void SelectBlock::OnTriggerEnter(REngine::HitContact& contact)
{
	contact;

	// UIを表示
	if (m_targetDialog) m_targetDialog->Show();
}

void SelectBlock::OnTriggerExit(REngine::HitContact & contact)
{
	contact;

	// UIを非表示
	if (m_targetDialog) m_targetDialog->Hide();
}
