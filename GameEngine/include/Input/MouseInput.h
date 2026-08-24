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

namespace REngine
{
	namespace Input
	{
		//====================================================//
		// クラス宣言
		//====================================================//
		class Mouse
		{
		public:
			// ボタンタイプ
			enum class Button
			{
				Left,
				Right,
				Middle,
			};

			// 軸タイプ
			enum class Axis
			{
				X, Y
			};

			// マウスステート
		private:
			inline static DirectX::Mouse::State m_nowMouseState;
			inline static DirectX::Mouse::State m_oldMouseState;

			inline static DirectX::SimpleMath::Vector2 m_oldMouseMove;	// 前フレームのマウスの移動量

		public:
			// ----- 関数宣言 ----- //
			Mouse();

			// キー情報の更新
			static void Update();

			// --- マウス情報取得関数 --- //

			// 押されたとき
			static inline bool GetDown(Button key)
			{
				return !GetButtonState(m_oldMouseState, key) && GetButtonState(m_nowMouseState, key);
			}
			// 押されているとき
			static inline bool Get(Button key)
			{
				return GetButtonState(m_nowMouseState, key);
			}
			static inline bool GetOld(Button key)
			{
				return GetButtonState(m_oldMouseState, key);
			}
			// 離されたとき
			static inline bool GetUp(Button key)
			{
				return GetButtonState(m_oldMouseState, key) && !GetButtonState(m_nowMouseState, key);
			}

			// 移動量
			static inline float GetMoveValue(Axis axis)
			{
				switch (axis)
				{
				case REngine::Input::Mouse::Axis::X:
					return GetMouseMoveValue().x;
				case REngine::Input::Mouse::Axis::Y:
					return GetMouseMoveValue().y;
				default:
					return 0;
				}
			}
			// 前フレームの移動量
			static inline float GetOldMoveValue(Axis axis)
			{
				switch (axis)
				{
				case REngine::Input::Mouse::Axis::X:
					return m_oldMouseMove.x;
				case REngine::Input::Mouse::Axis::Y:
					return m_oldMouseMove.y;
				default:
					return 0;
				}
			}

			// 位置
			static DirectX::SimpleMath::Vector2 GetMousePoint();
			static DirectX::SimpleMath::Vector2 GetOldMousePoint();

			// スケーリングされた位置
			static DirectX::SimpleMath::Vector2 GetScaledMousePoint();

			// 移動量
			static DirectX::SimpleMath::Vector2 GetMouseMoveValue();
			static int GetWheelMoveValue();

			// モードの取得
			static DirectX::Mouse::Mode GetMode();

			// モードの変更
			static void SetMode(DirectX::Mouse::Mode mode);

			// 位置のセット
			static void SetMousePoint(int x, int y);

			// マウスが映るかどうかをセット
			static void SetMouseVisible(bool visible);

		private:

			// 補助関数
			static bool GetButtonState(const DirectX::Mouse::State& state, Button button)
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
		};
	}
}	// namespace REngine
