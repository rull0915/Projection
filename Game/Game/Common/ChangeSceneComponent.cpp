//====================================================//
// ファイル名  : ChangeSceneComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/09/02
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ChangeSceneComponent.h"

#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"
#include "Input/PadInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ChangeSceneComponent::ChangeSceneComponent(REngine::IComponentOwner* own)
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

// 最初のUpdate関数の直線に一度呼ばれます
void ChangeSceneComponent::Start()
{
	// ボタンがあれば
	if (m_button)
	{
		// クリック時処理を追加
		m_eventToken = m_button->AddOnClicked([this]() { REngine::SceneManager::Instance().RequestSceneChange(m_targetScene, std::make_unique<REngine::Transition::Fade>(), std::make_unique<REngine::Transition::Fade>()); });
	}
}

void ChangeSceneComponent::Update(const REngine::GameTimer&)
{
	using namespace REngine::Input;

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
