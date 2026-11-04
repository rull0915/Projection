/**
 * @file    SwingComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/14
 */

#include "pch.h"
#include "SwingComponent.h"

SwingComponent::SwingComponent(RHEngine::IComponentOwner* pOwner)
	: UIComponentBase(pOwner)
	, m_rectTransform{ nullptr }
{
}

void SwingComponent::Awake()
{
	// RectTransformを取得
	m_rectTransform = GetComponent<RHEngine::RectTransform>();
}

void SwingComponent::Update(const RHEngine::GameTimer & gameTimer)
{
	static float e = 0;
	e += gameTimer.GetElapsedTime();

	// 揺らす
	m_rectTransform->SetRotation(std::sinf(e * 2) * 0.5f);
}
