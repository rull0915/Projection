//====================================================//
// ファイル名   : KeyInput.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : キー入力
//
// 更新履歴 :
// 2026/02/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <Keyboard.h>
#include "InputInfo.h"

namespace REngine
{
	namespace Input
	{
		class Key
		{
		public:
			using Code = DirectX::Keyboard::Keys;

		private:
			inline static DirectX::Keyboard::State m_nowState;
			inline static DirectX::Keyboard::State m_oldState;

		public:
			// ----- 関数宣言 ----- //

			Key();

			~Key() = default;

			// キー情報の更新
			static void Update();

			// キー情報取得関数
			static inline bool Get(State state, Code key)
			{
				switch (state)
				{
					// 押されたとき
				case Input::State::Down:
					return !m_oldState.IsKeyDown(key) && m_nowState.IsKeyDown(key);

					// 押されているとき
				case Input::State::Press:
					return m_nowState.IsKeyDown(key);

					// 離されたとき
				case Input::State::Up:
					return m_oldState.IsKeyDown(key) && !m_nowState.IsKeyDown(key);

				default:
					return false;
				}
			}
		};
	}
}	// namespace REngine
