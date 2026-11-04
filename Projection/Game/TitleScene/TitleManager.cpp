/**
 * @file    TitleManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/09/01
 */

#include "pch.h"
#include "TitleManager.h"

#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"
#include "GameController.h"
#include "Input/PadInput.h"

TitleManager::TitleManager(RHEngine::IComponentOwner* own)
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
	, m_audioSource{}
{
	ADD_PROPERTY(TitleManager, m_targetScene);
	ADD_PROPERTY(TitleManager, m_playButton);
	ADD_PROPERTY(TitleManager, m_exitButton);
	ADD_PROPERTY(TitleManager, m_exitAcceptButton);
	ADD_PROPERTY(TitleManager, m_exitCancelButton);
	ADD_PROPERTY(TitleManager, m_padUI);
	ADD_PROPERTY(TitleManager, m_padUIOnDialog);
	ADD_PROPERTY(TitleManager, m_quitDialog);
	ADD_PROPERTY(TitleManager, m_audioSource);
}

void TitleManager::Start()
{
	// 押下時処理を追加
	if (m_playButton)
	{
		m_playEventToken = m_playButton->AddOnClicked([this]() { 
			RHEngine::SceneManager::Instance().RequestSceneChange(m_targetScene, std::make_unique<RHEngine::Transition::Fade>(), std::make_unique<RHEngine::Transition::Fade>()); }
		);
	}
	// 押下時処理を追加
	if (m_exitAcceptButton)
	{
		m_exitEventToken = m_exitAcceptButton->AddOnClicked([]() { RHEngine::GameController::Instance().Quit(); });
	}

	// 音楽の再生
	if (m_audioSource) m_audioSource->Play();
}

void TitleManager::Update(const RHEngine::GameTimer &)
{
	// コントローラー接続チェック
	if (m_padUI)
	{
		m_padUI->SetActive(RHEngine::Input::Pad::IsConnected());
	}
	if (m_padUIOnDialog)
	{
		m_padUIOnDialog->SetActive(RHEngine::Input::Pad::IsConnected());
	}

	using namespace RHEngine::Input;

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
