/**
 * @file    SlideTransition.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   スライド式のシーン遷移
 */
#pragma once

#include "Scene/Transition/TransitionBase.h"

namespace Transition
{
	/**
	 * @brief スライド式シーン遷移
	 */
	class Slide : public RHEngine::Transition::Base
	{
	private:
		enum class End
		{
			RightUp,
			RightDown,
			LeftUp,
			LeftDown,
		};

	private:

		// 経過時間
		float m_elapsedTime;

		float m_transSec;

		DirectX::SimpleMath::Color m_slideColor;

		float m_slideAngle;

		// 最終系の4点座標
		DirectX::SimpleMath::Vector2 m_points[4];

	public:

		Slide(float transTime, DirectX::SimpleMath::Color slideColor = { 0, 0, 0, 1 }, float slideAngle = 0)
			: m_elapsedTime{ 0.0f }
			, m_transSec{ transTime }
			, m_slideColor{ slideColor }
			, m_slideAngle{ slideAngle }
			, m_points{}
		{}
		~Slide() = default;

		// 純粋仮想関数
		void Initialize() override;

		// 更新関数
		// 遷移終了時にtrue
		bool InUpdate(const RHEngine::GameTimer& gameTimer) override;	    // Inの場合	
		bool OutUpdate(const RHEngine::GameTimer& gameTimer) override;	// Outの場合	

		// 描画関数
		void InRender(RHEngine::Renderer& renderer) override;	    // Inの場合
		void OutRender(RHEngine::Renderer& renderer) override;	// Outの場合	
	};
}
