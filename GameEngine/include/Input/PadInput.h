/**
 * @file    PadInput.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/13
 *
 * @brief   ゲームパッドの入力クラス
 */

#pragma once

#include <GamePad.h>

namespace RHEngine
{
	namespace Input
	{
		/**
		 * @brief ゲームパッドの入力を管理する
		 */
		class Pad
		{
		public:
			/**
			 * @brief ボタンタイプ
			 */
			enum class Button
			{
				A, B, X, Y,
				LStick, RStick,
				LShoulder, RShoulder,
				View, Menu, 
				Up, Down, Left, Right
			};

			/**
			 * @brief スティックタイプ
			 */
			enum class Axis
			{
				LeftStickX,
				LeftStickY,
				RightStickX,
				RightStickY,
				LeftTrigger,
				RightTrigger,
			};

		private:

			/** @brief 現在の入力状態 */
			inline static DirectX::GamePad::State m_nowState;

			/** @brief 前フレームの入力状態 */
			inline static DirectX::GamePad::State m_oldState;

		public:

			/// @name ライフサイクル
			/// @{

			/**
			 * @details 静的クラスのためインスタンス化を禁止
			 */
			Pad() = delete;

			~Pad() = default;

			/// @}

			/**
			 * @brief 入力状況を更新する
			 */
			static void Update();

			/**
			 * @brief ゲームパッドが接続されているかを調べる
			 */
			static bool IsConnected() { return m_nowState.IsConnected(); }

			/**
			 * @brief ボタンが押されたフレームかどうか調べる
			 * @param button 対象のボタン
			 */
			static bool GetDown(Button button)
			{
				return !GetPressed(m_oldState, button) && GetPressed(m_nowState, button);
			}
			
			/**
			 * @brief ボタンが押されているかどうか調べる
			 * @param button 対象のボタン
			 */
			static bool Get(Button button)
			{
				return GetPressed(m_nowState, button);
			}

			/**
			 * @brief 前フレームにボタンが押されていたかどうか調べる
			 * @param button 対象のボタン
			 */
			static bool GetOld(Button button)
			{
				return GetPressed(m_oldState, button);
			}
			
			/**
			 * @brief ボタンが離されたフレームかどうか調べる
			 * @param button 対象のボタン
			 */
			static bool GetUp(Button button)
			{
				return GetPressed(m_oldState, button) && !GetPressed(m_nowState, button);
			}

			/**
			 * @brief スティックの入力を取得する
			 * @param stick 対象のスティック
			 */
			static float GetCurrentAxis(Axis stick)
			{
				return GetAxis(m_nowState, stick);
			}
			
			/**
			 * @brief 前フレームのスティックの入力を取得する
			 * @param stick 対象のスティック
			 */
			static float GetOldAxis(Axis stick)
			{
				return GetAxis(m_oldState, stick);
			}

			/**
			 * @brief ゲームパッドを振動させる
			 * @param left 左の強さ
			 * @param right 右の強さ
			 */
			static void SetVibration(float left, float right);

		private:
			/**
			 * @brief 指定したボタンの入力状態を取得する補助関数
			 * @param state 現在の入力状態
			 * @param button 対象のボタン
			 */
			static bool GetPressed(const DirectX::GamePad::State& state, Button button);

			/**
			 * @brief 指定したスティックの入力状態を取得する補助関数
			 * @param state 現在の入力状態
			 * @param stick 調べるスティック
			 */
			static float GetAxis(const DirectX::GamePad::State& state, Axis stick);
		};
	}
}	// namespace RHEngine
