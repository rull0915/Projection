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
#include "InputInfo.h"

namespace REngine
{
	namespace Input
	{
		class Pad
		{
		public:
			// ボタンタイプ
			enum class Button : unsigned char
			{
				A, B, X, Y,
				LStick, RStick,
				LShoulder, RShoulder,
				View, Menu, 
				Up, Down, Left, Right
			};

		private:
			inline static DirectX::GamePad::State m_nowState;
			inline static DirectX::GamePad::State m_oldState;

		public:
			// ----- 関数宣言 ----- //

			Pad();

			~Pad() = default;

			// キー情報の更新
			static void Update();

			// 指定したステートのボタンの押下状況を返す関数
			static bool GetPressed(const DirectX::GamePad::State& state, Button button);

			// キー情報取得関数
			static bool Get(State state, Button key);

			// 接続状況取得関数
			static bool IsConnected() { return m_nowState.IsConnected(); }
		};
	}
}	// namespace REngine
