/**
 * @file    GameTimer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/04
 */

#include "pch.h"
#include "Timer/GameTimer.h"

#include "Settings/TimeSettings.h"

namespace RHEngine
{
	void GameTimer::Update(float elapsedTime)
	{
		m_elapsedTime = elapsedTime * TimeSettings::Instance().GetTimeScale();
		m_unscaledElapsedTime = elapsedTime;

		m_time += m_elapsedTime;
		m_unscaledTime += m_unscaledElapsedTime;
	}
}	// namespace RHEngine
