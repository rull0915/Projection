/**
 * @file    KeyInput.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   キー入力
 */

#pragma once

#include <Keyboard.h>

namespace RHEngine
{
	namespace Input
	{
		/**
		 * @brief キー入力管理
		 */
		class Key
		{
		public:
			using Code = DirectX::Keyboard::Keys;

		private:

			/** @brief 現在の入力状況 */
			inline static DirectX::Keyboard::State m_nowState;

			/** @brief 前フレームの入力状況 */
			inline static DirectX::Keyboard::State m_oldState;

		private:

			/// @name ライフサイクル
			/// @{

			/**
			 * @details 静的クラスのためインスタンス化を禁止
			 */
			Key() = delete;

			/// @}

		public:

			/**
			 * @brief マウスの入力を更新する
			 */
			static void Update();

			/**
			 * @brief ボタンがおされたフレームかどうか
			 * @param key 対象のボタン
			 */
			static bool GetDown(Code key)
			{
				return !m_oldState.IsKeyDown(key) && m_nowState.IsKeyDown(key);
			}
			
			/**
			 * @brief ボタンが押されているか
			 * @param key 対象のボタン
			 */
			static bool Get(Code key)
			{
				return m_nowState.IsKeyDown(key);
			}

			/**
			 * @brief 前フレームに押されていたかどうか
			 * @param key 対象のボタン
			 */
			static bool GetOld(Code key)
			{
				return m_oldState.IsKeyDown(key);
			}

			/**
			 * @brief ボタンが離されたフレームかどうか
			 * @param key 対象のボタン
			 */
			static bool GetUp(Code key)
			{
				return m_oldState.IsKeyDown(key) && !m_nowState.IsKeyDown(key);
			}
		};
	}
}	// namespace RHEngine
