/**
 * @file    ChangeSceneComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/09/02
 */

#include "pch.h"
#include "ChangeSceneComponent.h"

#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"
#include "Input/PadInput.h"

ChangeSceneComponent::ChangeSceneComponent(RHEngine::IComponentOwner* own)
	: BothComponentBase(own)
	, m_button{}
	, m_targetScene{}
	, m_eventToken{}
	, m_padUI{}
{
	ADD_PROPERTY(ChangeSceneComponent, m_button);
	ADD_PROPERTY(ChangeSceneComponent, m_targetScene);
	ADD_PROPERTY(ChangeSceneComponent, m_padUI);
}

void ChangeSceneComponent::Start()
{
	// ボタンがあれば
	if (m_button)
	{
		// クリック時処理を追加
		m_eventToken = m_button->AddOnClicked([this]() { RHEngine::SceneManager::Instance().RequestSceneChange(m_targetScene, std::make_unique<RHEngine::Transition::Fade>(), std::make_unique<RHEngine::Transition::Fade>()); });
	}
}

void ChangeSceneComponent::Update(const RHEngine::GameTimer&)
{
	using namespace RHEngine::Input;

	bool padConnect = Pad::IsConnected();

	// 表示切替
	if (m_padUI) m_padUI->SetActive(padConnect);

	// コントローラーが繋がれているとき
	if (padConnect)
	{
		// Bボタンでシーン遷移
		if (Pad::GetDown(Pad::Button::B))
		{
			if (m_button) m_button->Submit();
		}
	}
}

void ChangeSceneComponent::OnDestroy()
{
	// ボタンがあれば
	if (m_button)
	{
		// クリック時処理を削除
		m_button->RemoveOnClicked(m_eventToken);
	}
}
