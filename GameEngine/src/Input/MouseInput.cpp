//====================================================//
// ファイル名  : MouseInput.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/02/17
//
// 概要        :  マウス入力管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Input/MouseInput.h"
#include "System/Render/GraphicsManager.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	namespace Input
	{
		// 更新関数
		void Mouse::Update()
		{
			// 前フレームの移動量を取得
			m_oldMouseMove = GetMouseMoveValue();

			// 前フレームの状態を保持
			m_oldMouseState = m_nowMouseState;

			// 最新の状態を取得
			auto& mouse = DirectX::Mouse::Get();
			m_nowMouseState = mouse.GetState();

			// モードの切り替え
			mouse.SetMode(m_requestedMode);
		}

		inline float Mouse::GetMoveValue(Axis axis)
		{
			switch (axis)
			{
			case RHEngine::Input::Mouse::Axis::X:
				return GetMouseMoveValue().x;
			case RHEngine::Input::Mouse::Axis::Y:
				return GetMouseMoveValue().y;
			default:
				return 0;
			}
		}

		inline float Mouse::GetOldMoveValue(Axis axis)
		{
			switch (axis)
			{
			case RHEngine::Input::Mouse::Axis::X:
				return m_oldMouseMove.x;
			case RHEngine::Input::Mouse::Axis::Y:
				return m_oldMouseMove.y;
			default:
				return 0;
			}
		}

		// マウスの位置を取得
		DirectX::SimpleMath::Vector2 Mouse::GetMousePoint()
		{
			int x = m_nowMouseState.x;
			int y = m_nowMouseState.y;

			return { static_cast<float>(x), static_cast<float>(y) };
		}

		// 前フレームの位置を取得
		DirectX::SimpleMath::Vector2 Mouse::GetOldMousePoint()
		{
			int x = m_oldMouseState.x;
			int y = m_oldMouseState.y;

			return { static_cast<float>(x), static_cast<float>(y) };
		}

		// マウスの移動量を取得
		DirectX::SimpleMath::Vector2 Mouse::GetMouseMoveValue()
		{
			// 絶対モードなら
			if (m_nowMouseState.positionMode == DirectX::Mouse::Mode::MODE_ABSOLUTE)
			{
				// 前フレームとの差分を返す
				return GetMousePoint() - GetOldMousePoint();
			}
			// 相対モードなら
			if (m_nowMouseState.positionMode == DirectX::Mouse::Mode::MODE_RELATIVE)
			{
				// 位置の戻り値が移動量のためそれを返す
				return GetMousePoint();
			}
			// 例外処理
			return { 0, 0 };
		}

		// ホイールの移動量を取得
		int Mouse::GetWheelMoveValue()
		{
			return m_nowMouseState.scrollWheelValue - m_oldMouseState.scrollWheelValue;
		}

		// マウスのモードをセット
		void Mouse::SetMode(DirectX::Mouse::Mode mode)
		{
			m_requestedMode = mode;
		}

		// マウスカーソルの位置をセットする関数
		void Mouse::SetMousePoint(int x, int y)
		{
			POINT pt = { x, y };
			HWND hwnd = GraphicsManager::Instance().GetDeviceResources()->GetWindow();

			ClientToScreen(hwnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}

		// マウスが映るかどうかを設定
		void Mouse::SetMouseVisible(bool visible)
		{
			DirectX::Mouse::Get().SetVisible(visible);
		}

		bool Mouse::GetButtonState(const DirectX::Mouse::State& state, Button button)
		{
			switch (button)
			{
			case Input::Mouse::Button::Left:
				return state.leftButton;

			case Input::Mouse::Button::Right:
				return state.rightButton;

			case Input::Mouse::Button::Middle:
				return state.middleButton;

			default:
				return false;
			}
		}
	}
}	// namespace RHEngine
