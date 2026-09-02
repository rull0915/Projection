//====================================================//
// ファイル名  : TitleManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/09/01
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TitleManager.h"

#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"
#include "GameController.h"
#include "Input/PadInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
TitleManager::TitleManager(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_targetScene{}
	, m_playButton{}
	, m_exitButton{}
	, m_exitAcceptButton{}
	, m_exitCancelButton{}
	, m_playEventToken{}
	, m_exitEventToken{}
	, m_padUI{}
	, m_padUIOnDialog{}
	, m_quitDialog{}
{
	ADD_PROPERTY(m_targetScene);
	ADD_PROPERTY(m_playButton);
	ADD_PROPERTY(m_exitButton);
	ADD_PROPERTY(m_exitAcceptButton);
	ADD_PROPERTY(m_exitCancelButton);
	ADD_PROPERTY(m_padUI);
	ADD_PROPERTY(m_padUIOnDialog);
	ADD_PROPERTY(m_quitDialog);
}

// 最初のUpdate関数の直線に一度呼ばれます
void TitleManager::Start()
{
	// 押下時処理を追加
	if (m_playButton)
	{
		m_playEventToken = m_playButton->AddOnClicked([this]() { 
			REngine::SceneManager::Instance().RequestSceneChange(m_targetScene, std::make_unique<REngine::Transition::Fade>(), std::make_unique<REngine::Transition::Fade>()); }
		);
	}
	// 押下時処理を追加
	if (m_exitAcceptButton)
	{
		m_exitEventToken = m_exitAcceptButton->AddOnClicked([]() { REngine::GameController::Instance().Quit(); });
	}
}

// 毎フレーム呼ばれます
void TitleManager::Update(const REngine::GameTimer &)
{
	// コントローラー接続チェック
	if (m_padUI)
	{
		m_padUI->SetActive(REngine::Input::Pad::IsConnected());
	}
	if (m_padUIOnDialog)
	{
		m_padUIOnDialog->SetActive(REngine::Input::Pad::IsConnected());
	}

	using namespace REngine::Input;

	// ダイアログが開かれていないとき
	if (m_quitDialog && !m_quitDialog->IsOpen())
	{
		// padの各入力を調べる
		if (Pad::GetDown(Pad::Button::B))
		{
			if (m_playButton) m_playButton->Submit();
		}
		if (Pad::GetDown(Pad::Button::A))
		{
			if (m_exitButton) m_exitButton->Submit();
		}
	}
	// 開かれているとき
	else if (m_quitDialog && m_quitDialog->IsOpen())
	{
		// padの各入力を調べる
		if (Pad::GetDown(Pad::Button::B))
		{
			if (m_exitAcceptButton) m_exitAcceptButton->Submit();
		}
		if (Pad::GetDown(Pad::Button::A))
		{
			if (m_exitCancelButton) m_exitCancelButton->Submit();
		}
	}
}

void TitleManager::OnDestroy()
{
	if (m_playButton) m_playButton->RemoveOnClicked(m_playEventToken);
	if (m_exitButton) m_exitButton->RemoveOnClicked(m_exitEventToken);
}
