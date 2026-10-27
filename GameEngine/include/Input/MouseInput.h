/**
 * @file    MouseInput.h
 * @author  Hoshino Ryunosuke
 * @date    2026/02/17
 *
 * @brief   マウス入力管理クラス
 */

#pragma once

#include <Mouse.h>

namespace RHEngine
{
	namespace Input
	{
		/**
		 * @brief マウス入力管理クラス
		 */
		class Mouse
		{
		public:
			/**
			 * @brief マウスボタン
			 */
			enum class Button
			{
				Left,
				Right,
				Middle,
			};

			/**
			 * @brief マウス移動の軸
			 */
			enum class Axis
			{
				X, Y
			};

		private:

			/** @brief 現在のマウス状態 */
			inline static DirectX::Mouse::State m_nowMouseState;

			/** @brief 前フレームのマウス状態 */
			inline static DirectX::Mouse::State m_oldMouseState;

			/** @brief 前フレームのマウス移動量 */
			inline static DirectX::SimpleMath::Vector2 m_oldMouseMove;

			/** @brief 切り替え要請中のモード */
			inline static DirectX::Mouse::Mode m_requestedMode;

		public:

			/// @name ライフサイクル
			/// @{

			/**
			 * @details 静的クラスのためインスタンス化を禁止
			 */
			Mouse() = delete;

			/// @}

			/**
			 * @brief マウス状態の更新x
			 */
			static void Update();

			/**
			 * @brief 押されたフレームかどうか
			 * @param key 対象のボタン
			 */
			static inline bool GetDown(Button key)
			{
				return !GetButtonState(m_oldMouseState, key) && GetButtonState(m_nowMouseState, key);
			}
			
			/**
			 * @brief 押されているかどうか
			 * @param key 対象のボタン
			 */
			static inline bool Get(Button key)
			{
				return GetButtonState(m_nowMouseState, key);
			}

			/**
			 * @brief 前フレームに押されていたかどうか
			 * @param key 対象のボタン
			 */
			static inline bool GetOld(Button key)
			{
				return GetButtonState(m_oldMouseState, key);
			}
			
			/**
			 * @brief 離されたフレームかどうか
			 * @param key 対象のボタン
			 */
			static inline bool GetUp(Button key)
			{
				return GetButtonState(m_oldMouseState, key) && !GetButtonState(m_nowMouseState, key);
			}

			/**
			 * @brief マウスの移動量を取得する
			 * @param axis 取得する軸
			 */
			static inline float GetMoveValue(Axis axis);

			/**
			 * @brief 前フレームの移動量を取得する
			 * @param axis 取得する軸
			 */
			static inline float GetOldMoveValue(Axis axis);

			/**
			 * @brief マウスのピクセル座標を取得する
			 */
			static DirectX::SimpleMath::Vector2 GetMousePoint();

			/**
			 * @brief 前フレームのマウスのピクセル座標を取得する
			 */
			static DirectX::SimpleMath::Vector2 GetOldMousePoint();

			/**
			 * @brief マウスの移動量を取得する
			 */
			static DirectX::SimpleMath::Vector2 GetMouseMoveValue();

			/**
			 * @brief ホイールの移動量を取得する
			 */
			static int GetWheelMoveValue();

			/**
			 * @brief 現在のモードを取得する
			 */
			static DirectX::Mouse::Mode GetMode() { return m_requestedMode; }

			/**
			 * @brief モードを変更する
			 * @param mode 変更対象のモード
			 */
			static void SetMode(DirectX::Mouse::Mode mode);

			/**
			 * @brief 強制的に位置をセットする
			 * @param x セットするピクセルX座標
			 * @param y セットするピクセルY座標
			 */
			static void SetMousePoint(int x, int y);

			/**
			 * @brief マウスが見えるかどうかをセットする
			 */
			static void SetMouseVisible(bool visible);

		private:

			/**
			 * @brief ボタンの状態を取得する補助関数
			 * @param state 現在の状態
			 * @param button 取得するボタン
			 */
			static bool GetButtonState(const DirectX::Mouse::State& state, Button button);
		};
	}
}	// namespace RHEngine
