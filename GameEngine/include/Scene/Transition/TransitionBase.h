/**
 * @file    SceneTransition.h
 * @author  Hoshino Ryunosuke
 * @date    2025/12/18
 *
 * @brief   シーン遷移の基底クラス
 */

#pragma once

namespace RHEngine
{
	class Renderer;

	namespace Transition
	{
		/**
		 * @brief 遷移モード
		 */
		enum class Mode
		{
			None,
			In,
			Out
		};

		/**
		 * @brief シーン遷移基底
		 */
		class Base
		{
		public:

			/// @name ライフサイクル
			/// @{

			Base() = default;

			virtual ~Base() = default;

			/// @}

			/**
			 * @brief 初期化 各遷移開始前に呼ばれる
			 */
			virtual void Initialize() = 0;

			/**
			 * @brief In状態の更新
			 * @details 画面がだんだん見えていく演出の更新処理
			 * @param gameTimer タイマー
			 * @return 遷移が終了したかどうか
			 */
			virtual bool InUpdate(const GameTimer& gameTimer) = 0;

			/**
			 * @brief Out状態の更新
			 * @details 画面がだんだん隠れていく演出の更新処理
			 * @param gameTimer タイマー
			 * @return 遷移が終了したかどうか
			 */
			virtual bool OutUpdate(const GameTimer& gameTimer) = 0;

			/**
			 * @brief In状態の描画処理
			 */
			virtual void InRender(Renderer& renderer) = 0;

			/**
			 * @brief Out状態の描画処理
			 */
			virtual void OutRender(Renderer& renderer) = 0;
		};
	}
}	// namespace RHEngine
