/**
 * @file    KeyInput.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/02/16
 */

#include "pch.h"
#include "Input/KeyInput.h"

namespace RHEngine
{
	namespace Input
	{
		// 更新関数
		void Key::Update()
		{
			// 前フレームの情報を保持
			m_oldState = m_nowState;

			// 最新の情報を取得
			m_nowState = DirectX::Keyboard::Get().GetState();
		}
	}
}	// namespace RHEngine
