/**
 * @file    PadInput.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/13
 */

#include "pch.h"
#include "Input/PadInput.h"

#include "GamePad.h"

namespace RHEngine
{
	namespace Input
	{
		Pad::Pad()
		{}

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
			case RHEngine::Input::Pad::Button::A:
				return state.IsAPressed();
			case RHEngine::Input::Pad::Button::B:
				return state.IsBPressed();
			case RHEngine::Input::Pad::Button::X:
				return state.IsXPressed();
			case RHEngine::Input::Pad::Button::Y:
				return state.IsYPressed();
			case RHEngine::Input::Pad::Button::LStick:
				return state.IsLeftStickPressed();
			case RHEngine::Input::Pad::Button::RStick:
				return state.IsRightStickPressed();
			case RHEngine::Input::Pad::Button::LShoulder:
				return state.IsLeftShoulderPressed();
			case RHEngine::Input::Pad::Button::RShoulder:
				return state.IsRightShoulderPressed();
			case RHEngine::Input::Pad::Button::View:
				return state.IsViewPressed();
			case RHEngine::Input::Pad::Button::Menu:
				return state.IsMenuPressed();
			case RHEngine::Input::Pad::Button::Up:
				return state.IsDPadUpPressed();
			case RHEngine::Input::Pad::Button::Down:
				return state.IsDPadDownPressed();
			case RHEngine::Input::Pad::Button::Left:
				return state.IsDPadLeftPressed();
			case RHEngine::Input::Pad::Button::Right:
				return state.IsDPadRightPressed();
			default:
				return false;
			}
		}

		float Pad::GetAxis(const DirectX::GamePad::State& state, Axis stick)
		{
			switch (stick)
			{
			case RHEngine::Input::Pad::Axis::LeftStickX:
				return state.thumbSticks.leftX;
			case RHEngine::Input::Pad::Axis::LeftStickY:
				return state.thumbSticks.leftY;
			case RHEngine::Input::Pad::Axis::RightStickX:
				return state.thumbSticks.rightX;
			case RHEngine::Input::Pad::Axis::RightStickY:
				return state.thumbSticks.rightY;
			case RHEngine::Input::Pad::Axis::LeftTrigger:
				return state.triggers.left;
			case RHEngine::Input::Pad::Axis::RightTrigger:
				return state.triggers.right;
			default:
				return 0;
			}
		}

		void Pad::SetVibration(float left, float right)
		{
			DirectX::GamePad::Get().SetVibration(0, left, right);
		}
	}
}	// namespace RHEngine
