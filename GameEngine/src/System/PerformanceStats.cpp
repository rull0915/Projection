/**
 * @file    PerformanceStats.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 */

#include "pch.h"
#include "System/PerformanceStats.h"

namespace RHEngine
{
	void PerformanceStats::Update(float elapsedTime)
	{
		// FPSを計測
		MeasureFPS(elapsedTime);
	}

	void PerformanceStats::MeasureFPS(float elapsedTime)
	{
		// 時間、フレーム数の加算
		m_timeAccumulator += elapsedTime;
		++m_frameCount;

		// 1秒ごとに更新
		if (m_timeAccumulator > 1.0f)
		{
			// FPSの計測
			m_fps = static_cast<float>(m_frameCount) / m_timeAccumulator;

			// リセット
			m_frameCount = 0;
			m_timeAccumulator = 0;
		}
	}
}	// namespace RHEngine
