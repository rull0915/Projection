/**
 * @file    FadeTransition.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 */

#include "pch.h"
#include "Scene/Transition/FadeTransition.h"

#include "Renderer/Renderer.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	namespace Transition
	{
		void Fade::Initialize()
		{
			m_elapsedTime = 0.0f;
		}

		bool Fade::InUpdate(const GameTimer& gameTimer)
		{
			// 経過時間の加算
			m_elapsedTime += gameTimer.GetUnScaledElapsedTime();

			// 補正
			if (m_elapsedTime >= m_transSec) m_elapsedTime = m_transSec;

			// 遷移時間を超えたかどうか
			return (m_elapsedTime >= m_transSec);
		}

		bool Fade::OutUpdate(const GameTimer& gameTimer)
		{
			// 経過時間の加算
			m_elapsedTime += gameTimer.GetUnScaledElapsedTime();

			// 補正
			if (m_elapsedTime >= m_transSec) m_elapsedTime = m_transSec;

			// 遷移時間を超えたかどうか
			return (m_elapsedTime >= m_transSec);
		}

		void Fade::InRender(Renderer& renderer)
		{
			// 透明度を計算
			float alpha = 255 - 255 * (m_elapsedTime / m_transSec);

			// 設定
			m_fadeColor.w = (alpha / 255.0f);

			// 描画
			renderer.Draw().UI().DrawBox({ 0, 0 }, { WindowManager::Instance().GetWidthF(), WindowManager::Instance().GetHeightF() }, m_fadeColor, true);
		}

		void Fade::OutRender(Renderer& renderer)
		{
			// 透明度を計算
			float alpha = 255 * (m_elapsedTime / m_transSec);

			// 設定
			m_fadeColor.w = (alpha / 255.0f);

			// 描画
			renderer.Draw().UI().DrawBox({ 0, 0 }, { WindowManager::Instance().GetWidthF(), WindowManager::Instance().GetHeightF() }, m_fadeColor, true);
		}
	}
}	// namespace RHEngine
