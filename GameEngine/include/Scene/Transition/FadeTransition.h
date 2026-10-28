/**
 * @file    FadeTransition.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   フェードのシーン遷移
 */

#pragma once

#include <SimpleMath.h>

#include "TransitionBase.h"

namespace RHEngine
{
	class GameTimer;
	class Renderer;

	namespace Transition
	{
		/**
		 * @brief フェードトランジション
		 */
		class Fade : public Base
		{
		private:

			/** @brief 経過時間 */
			float m_elapsedTime;

			/** @brief 遷移にかかる時間 */
			float m_transSec;

			/** @brief フェードの色 */
			DirectX::SimpleMath::Color m_fadeColor;

		public:

			/// @name ライフサイクル
			/// @{

			/**
			 * @param sec 遷移にかかる時間
			 * @param color フェード色
			 */
			Fade(float sec = 0.5f, DirectX::SimpleMath::Color color = { 0, 0, 0, 1 })
				: m_transSec{ sec }
				, m_fadeColor{ color }
				, m_elapsedTime{ 0.0f }
			{}

			~Fade() = default;

			/// @}

			/**
			 * @brief 経過時間の初期化
			 */
			void Initialize() override;

			/**
			 * @brief フェードイン時の更新
			 * @param gameTimer タイマー
			 * @return 終了したかどうか
			 */
			bool InUpdate(const GameTimer& gameTimer) override;

			/**
			 * @brief フェードアウト時の更新
			 * @param gameTimer タイマー
			 * @return 終了したかどうか
			 */
			bool OutUpdate(const GameTimer& gameTimer) override;

			/**
			 * @brief フェードイン時の描画
			 */
			void InRender(Renderer& renderer) override;

			/**
			 * @brief フェードアウト時の描画
			 */
			void OutRender(Renderer& renderer) override;
		};
	}
}	// namespace RHEngine
