//====================================================//
// ファイル名  : PadInput.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/13
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Input/PadInput.h"
#include "GamePad.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	namespace Input
	{
		// コンストラクタ
		Pad::Pad()
		{}

		// 更新関数
		void Pad::Update()
		{
			// 前フレームの情報を保持
			m_oldState = m_nowState;

			// 最新の情報を取得
			m_nowState = DirectX::GamePad::Get().GetState(0);
		}

		bool Pad::GetPressed(const DirectX::GamePad::State& state, Button button)
		{
			// 接続チェック
			if (!state.IsConnected()) return false;

			switch (button)
			{
			case REngine::Input::Pad::Button::A:
				return state.IsAPressed();
			case REngine::Input::Pad::Button::B:
				return state.IsBPressed();
			case REngine::Input::Pad::Button::X:
				return state.IsXPressed();
			case REngine::Input::Pad::Button::Y:
				return state.IsYPressed();
			case REngine::Input::Pad::Button::LStick:
				return state.IsLeftStickPressed();
			case REngine::Input::Pad::Button::RStick:
				return state.IsRightStickPressed();
			case REngine::Input::Pad::Button::LShoulder:
				return state.IsLeftShoulderPressed();
			case REngine::Input::Pad::Button::RShoulder:
				return state.IsRightShoulderPressed();
			case REngine::Input::Pad::Button::View:
				return state.IsViewPressed();
			case REngine::Input::Pad::Button::Menu:
				return state.IsMenuPressed();
			case REngine::Input::Pad::Button::Up:
				return state.IsDPadUpPressed();
			case REngine::Input::Pad::Button::Down:
				return state.IsDPadDownPressed();
			case REngine::Input::Pad::Button::Left:
				return state.IsDPadLeftPressed();
			case REngine::Input::Pad::Button::Right:
				return state.IsDPadRightPressed();
			default:
				return false;
			}
		}

		bool Pad::Get(State state, Button key)
		{
			// 状態によって分岐
			switch (state)
			{
			case REngine::Input::State::Down:
				return !GetPressed(m_oldState, key) && GetPressed(m_nowState, key);
			case REngine::Input::State::Press:
				return GetPressed(m_nowState, key);
			case REngine::Input::State::Up:
				return GetPressed(m_oldState, key) && !GetPressed(m_nowState, key);
			default:
				return false;
			}
		}
	}
}	// namespace REngine
