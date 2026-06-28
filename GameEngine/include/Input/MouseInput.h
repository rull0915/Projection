//====================================================//
// ファイル名   : MouseInput.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/02/17
//
// 概要 : マウス入力管理クラス
//
// 更新履歴 :
// 2026/02/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================// 

#include <Mouse.h>
#include "InputInfo.h"

namespace Input
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Mouse
	{
	public:
		// ボタンタイプ
		enum class Button : unsigned char
		{
			Left,
			Right,
			Middle,
		};

		// マウスステート
	private:
		inline static DirectX::Mouse::State m_nowMouseState;
		inline static DirectX::Mouse::State m_oldMouseState;

	public:
		// ----- 関数宣言 ----- //
		Mouse();

		// キー情報の更新
		static void Update();

		// --- マウス情報取得関数 --- //

		static bool Get(State state, Button button);

		// 位置
		static DirectX::SimpleMath::Vector2 GetMousePoint();
		static DirectX::SimpleMath::Vector2 GetOldMousePoint();

		// スケーリングされた位置
		static DirectX::SimpleMath::Vector2 GetScaledMousePoint();

		// 移動量
		static DirectX::SimpleMath::Vector2 GetMouseMoveValue();
		static int GetWheelMoveValue();

		// モードの変更
		static void SetMode(DirectX::Mouse::Mode mode);

		// 位置のセット
		static void SetMousePoint(int x, int y);

		// マウスが映るかどうかをセット
		static void SetMouseVisible(bool visible);

	private:

		// 補助関数
		static bool GetButtonState(Button button)
		{
			switch (button)
			{
			case Input::Mouse::Button::Left:
				return m_nowMouseState.leftButton;
	
			case Input::Mouse::Button::Right:
				return m_nowMouseState.rightButton;
	
			case Input::Mouse::Button::Middle:
				return m_nowMouseState.middleButton;
	
			default:
				return false;
			}
		}
		static bool GetOldButtonState(Button button)
		{
			switch (button)
			{
			case Input::Mouse::Button::Left:
				return m_oldMouseState.leftButton;
				
			case Input::Mouse::Button::Right:
				return m_oldMouseState.rightButton;
				
			case Input::Mouse::Button::Middle:
				return m_oldMouseState.middleButton;
				
			default:
				return false;
			}
		}
	};
}
