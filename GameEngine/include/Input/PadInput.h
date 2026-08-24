//====================================================//
// ファイル名   : PadInput.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/13
//
// 概要 : ゲームパッドの入力クラス
//
// 更新履歴 :
// 2026/08/13 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <GamePad.h>

namespace REngine
{
	namespace Input
	{
		class Pad
		{
		public:
			// ボタンタイプ
			enum class Button
			{
				A, B, X, Y,
				LStick, RStick,
				LShoulder, RShoulder,
				View, Menu, 
				Up, Down, Left, Right
			};

			// スティックタイプ
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
			inline static DirectX::GamePad::State m_nowState;
			inline static DirectX::GamePad::State m_oldState;

		public:
			// ----- 関数宣言 ----- //

			Pad();

			~Pad() = default;

			// 更新
			static void Update();

			// 接続状況取得関数
			static bool IsConnected() { return m_nowState.IsConnected(); }

			//----- ボタンの押下状況取得関数 -----//

			// 押されたとき
			static inline bool GetDown(Button button)
			{
				return !GetPressed(m_oldState, button) && GetPressed(m_nowState, button);
			}
			// 押されているとき
			static inline bool Get(Button button)
			{
				return GetPressed(m_nowState, button);
			}
			static inline bool GetOld(Button button)
			{
				return GetPressed(m_oldState, button);
			}
			// 離されたとき
			static inline bool GetUp(Button button)
			{
				return GetPressed(m_oldState, button) && !GetPressed(m_nowState, button);
			}

			//----- スティックの押下状況取得関数 -----//

			// 今のフレーム
			static inline float GetCurrentAxis(Axis stick)
			{
				return GetAxis(m_nowState, stick);
			}
			
			// 前フレーム
			static inline float GetOldAxis(Axis stick)
			{
				return GetAxis(m_oldState, stick);
			}

		private:
			// 指定したステートのボタンの押下状況を返す関数
			static bool GetPressed(const DirectX::GamePad::State& state, Button button);

			// 指定したステートのスティックの状況を返す関数
			static float GetAxis(const DirectX::GamePad::State& state, Axis stick);
		};
	}
}	// namespace REngine
