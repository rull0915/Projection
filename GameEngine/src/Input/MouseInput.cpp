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
#include "System/ResourceManager.h"
#include "System/DeviceResources.h"
#include "System/WindowManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	namespace Input
	{
		// コンストラクタ
		Mouse::Mouse()
		{
			// 初期状態を取得
			m_oldMouseState = m_nowMouseState;
			m_nowMouseState = DirectX::Mouse::Get().GetState();
		}

		// 更新関数
		void Mouse::Update()
		{
			// 前フレームの状態を保持
			m_oldMouseState = m_nowMouseState;

			// 最新の状態を取得
			m_nowMouseState = DirectX::Mouse::Get().GetState();
		}

		bool Mouse::Get(State state, Button button)
		{
			switch (state)
			{
				// 押された場合
			case Input::State::Down:
				return !GetOldButtonState(button) && GetButtonState(button);

				// 押されている場合	
			case Input::State::Press:
				return GetButtonState(button);

				// 離された場合
			case Input::State::Up:
				return GetOldButtonState(button) && !GetButtonState(button);

			default:
				return false;
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

		// スケールされたマウス座標を取得 (フルスクリーン化時に使用)
		DirectX::SimpleMath::Vector2 Mouse::GetScaledMousePoint()
		{
			// 今の画面サイズを取得
			RECT rc;
			GetClientRect(ResourceManager::Instance().GetResources()->GetWindow(), &rc);

			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;

			// 割合を取得
			float ratioX = WindowManager::Instance().GetWidthF() / width;
			float ratioY = WindowManager::Instance().GetHeightF() / height;

			return GetMousePoint() * DirectX::SimpleMath::Vector2{ ratioX, ratioY };
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

		// マウスのモードを取得
		DirectX::Mouse::Mode Mouse::GetMode()
		{
			return m_nowMouseState.positionMode;
		}

		// マウスのモードをセット
		void Mouse::SetMode(DirectX::Mouse::Mode mode)
		{
			DirectX::Mouse::Get().SetMode(mode);
		}

		// マウスカーソルの位置をセットする関数
		void Mouse::SetMousePoint(int x, int y)
		{
			POINT pt = { x, y };
			HWND hwnd = ResourceManager::Instance().GetResources()->GetWindow();

			ClientToScreen(hwnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}

		// マウスが映るかどうかを設定
		void Mouse::SetMouseVisible(bool visible)
		{
			DirectX::Mouse::Get().SetVisible(visible);
		}
	}
}	// namespace REngine
