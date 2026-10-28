/**
 * @file    ClearEffect.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/13
 */

#include "pch.h"
#include "ClearEffect.h"

#include "Scene/SceneManager.h"
#include "Settings/TimeSettings.h"
#include "GameLib/Transition/SlideTransition.h"

ClearEffect::ClearEffect(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_clearUIName{}
	, m_clearDialog{}
	, m_toClearTime{ 1.0f }
	, m_sumTime{ 0 }
	, m_claerTimeScale{ 0.2f }
{
	ADD_PROPERTY(ClearEffect, m_clearUIName);
	ADD_PROPERTY(ClearEffect, m_clearDialog);
	ADD_PROPERTY(ClearEffect, m_toClearTime);
	ADD_PROPERTY(ClearEffect, m_claerTimeScale);
}

void ClearEffect::Awake()
{
	// スローに
	RHEngine::TimeSettings::Instance().SetTimeScale(1.0f);
}

void ClearEffect::Start()
{
	// 時間の初期化
	m_sumTime = 0;

	// スローに
	RHEngine::TimeSettings::Instance().SetTimeScale(m_claerTimeScale);

	// ダイアログを出現
	if (m_clearDialog) m_clearDialog->Show();
}

void ClearEffect::OnDestroy()
{
	// 時間設定を戻す
	RHEngine::TimeSettings::Instance().SetTimeScale(1.0f);
}

void ClearEffect::Update(const RHEngine::GameTimer & gameTimer)
{
	// TimeScaleに影響されない時間を加算する
	m_sumTime += gameTimer.GetUnScaledElapsedTime();

	// ボーダーを超えたとき
	if (m_sumTime > m_toClearTime)
	{
		// クリアシーンへの移行をリクエスト
		RHEngine::SceneManager::Instance().RequestSceneChange("Clear",
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(30)),
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(210))
			);
	}
}
