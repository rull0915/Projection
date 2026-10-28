/**
 * @file    PauseDialog.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/30
 */

#include "pch.h"
#include "PauseDialog.h"

PauseDialog::PauseDialog(RHEngine::IComponentOwner* own)
	: Dialog(own)
	, m_openEasing{}
	, m_closeEasing{}
	, m_rectTransform{ nullptr }
	, m_transTime{ 0 }
	, m_expType{ Expansion::X }
	, m_currentMode{ Mode::Hide }
	, m_currentTime{ 0 }

{
	ADD_PROPERTY(PauseDialog, m_openEasing);
	ADD_PROPERTY(PauseDialog, m_closeEasing);
	ADD_PROPERTY(PauseDialog, m_transTime);
	ADD_PROPERTY(PauseDialog, m_expType);
}

void PauseDialog::Start()
{
	// rectを取得
	m_rectTransform = GetComponent<RHEngine::RectTransform>();

	// 初期状態のスケールを取得
	m_initScale = m_rectTransform->GetLocalScale();

	// 隠す
	m_rectTransform->SetScale({ 0, 0 });
}

void PauseDialog::Update(const RHEngine::GameTimer & gameTimer)
{
	// モードによって変更
	switch (m_currentMode)
	{
	case PauseDialog::Mode::Showing:

		// 時間を加算
		m_currentTime += gameTimer.GetElapsedTime();

		// クランプ
		m_currentTime = std::clamp(m_currentTime, 0.0f, m_transTime);

		// Rectを変更
		ChangeScale(m_currentTime / m_transTime);

		// 遷移時間を超えたら
		if (m_currentTime >= m_transTime)
		{
			// Showへ
			m_currentMode = Mode::Show;
		}

		break;
	case PauseDialog::Mode::Hiding:
		
		// 時間を減算
		m_currentTime -= gameTimer.GetElapsedTime();

		// クランプ
		m_currentTime = std::clamp(m_currentTime, 0.0f, m_transTime);

		// Rectを変更
		ChangeScale(m_currentTime / m_transTime);

		// 遷移時間を超えたら
		if (m_currentTime <= 0)
		{
			// Hideへ
			m_currentMode = Mode::Hide;
		}

		break;
	default:
		break;
	}
}

void PauseDialog::Show()
{
	if (IsOpen()) return;

	SetOpen(true);

	// モードを開き中に
	m_currentMode = Mode::Showing;
}

void PauseDialog::Hide()
{
	if (!IsOpen()) return;

	SetOpen(false);

	// モードを閉じ中に
	m_currentMode = Mode::Hiding;
}

void PauseDialog::ChangeScale(float ratio)
{
	// 初期スケールを取得
	DirectX::SimpleMath::Vector2 nextScale = m_initScale;

	// イージングを使用しtを取得
	float t = 0;
	if (m_currentMode == Mode::Showing) t = m_openEasing.Get(ratio);
	if (m_currentMode == Mode::Hiding) t = m_closeEasing.Get(ratio);

	// 拡大モードに応じて変更
	if (m_expType == Expansion::X || m_expType == Expansion::XY)
	{
		nextScale.x *= t;
	}
	if (m_expType == Expansion::Y || m_expType == Expansion::XY)
	{
		nextScale.y *= t;
	}

	// セット
	m_rectTransform->SetScale(nextScale);
}
