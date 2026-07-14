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
#include "Scene/Scene.h"
#include "Scene/Transition/FadeTransition.h"
#include "Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SelectBlock::SelectBlock(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_targetScene{}
	, m_uiName{}
	, m_uiObject{ nullptr }
{
	ADD_PROPERTY(m_targetScene);
	ADD_PROPERTY(m_uiName);
}

void SelectBlock::Start()
{
	// UIオブジェクトを取得
	m_uiObject =
		static_cast<GameObject*>(GetOwn())->GetScene()->GetObjectFinder()->FindWithNameInUI(m_uiName);
}

void SelectBlock::OnTriggerStay(HitContact & contact)
{
	// プレイヤーが当たっているとき
	if (contact.other->GetTag() == "Player")
	{
		// スペースキーでシーン移動
		if (Input::Key::Get(Input::State::Down, Input::Key::Code::Space))
		{
			SceneManager::Instance().RequestSceneChange(m_targetScene,
				std::make_unique<Transition::Fade>(),
				std::make_unique<Transition::Fade>()
				);
		}
	}
}

void SelectBlock::OnTriggerEnter(HitContact& contact)
{
	contact;

	// UIを表示
	if (m_uiObject) m_uiObject->SetActive(true);
}

void SelectBlock::OnTriggerExit(HitContact & contact)
{
	contact;

	// UIを非表示
	if (m_uiObject) m_uiObject->SetActive(false);
}
