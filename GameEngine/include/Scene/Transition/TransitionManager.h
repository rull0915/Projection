/**
 * @file    TransitionManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   シーン遷移管理クラス
 */

#pragma once

#include <memory>
#include <functional>

#include "TransitionBase.h"

namespace RHEngine
{
	class GameTimer;
	class Renderer;

	/**
	 * @brief シーン遷移管理
	 */
	class TransitionManager
	{
	private:

		/** @brief 今のモード */
		Transition::Mode m_nowMode;

		/** @brief Out演出 */
		std::unique_ptr<Transition::Base> m_outTransition;

		/** @brief In演出 */
		std::unique_ptr<Transition::Base> m_inTransition;

		/** @brief Out終了時,Inの直前に呼ばれる関数 */
		std::function<void()> m_func;

	public:

		/// @name ライフサイクル
		/// @{

		TransitionManager()
			: m_outTransition{}
			, m_inTransition{}
			, m_func{}
			, m_nowMode{ Transition::Mode::None }
		{}

		~TransitionManager() = default;

		/// @}

		/**
		 * @brief 遷移演出の更新を行う
		 * @param gameTimer タイマー
		 */
		void Update(const GameTimer& gameTimer);

		/**
		 * @brief 描画演出の描画を行う
		 */
		void Render(Renderer& renderer);

		/**
		 * @brief 遷移演出を開始する
		 * @param outTrans 画面を隠すエフェクト
		 * @param inTrans 画面を見せるエフェクト
		 * @param func シーンを実際に切りかえる関数
		 */
		void StartTrans(
			std::unique_ptr<Transition::Base> outTrans,
			std::unique_ptr<Transition::Base> inTrans,
			std::function<void()> func
		);

		/**
		 * @brief シーン遷移演出中かどうか
		 */
		bool IsTransitioning() const;
	};
}	// namespace RHEngine
