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
#include "MouseInput.h"
#include "GameLib/Resources/ResourceManager.h"
#include "DeviceResources.h"

//====================================================//
// 関数の実体宣言
//====================================================//

DirectX::Mouse::State MouseInput::m_nowMouseState;
DirectX::Mouse::State MouseInput::m_oldMouseState;

MouseInput::MouseInput()
{
	m_oldMouseState = m_nowMouseState;
	m_nowMouseState = DirectX::Mouse::Get().GetState();
}

void MouseInput::MouseUpdate()
{
	m_oldMouseState = m_nowMouseState;
	m_nowMouseState = DirectX::Mouse::Get().GetState();
}

bool MouseInput::GetMouseDown(int MouseCode)
{
	switch (MouseCode)
	{
	case MOUSE_LEFT:
		return !m_oldMouseState.leftButton && m_nowMouseState.leftButton;
		break;
	case MOUSE_MIDDLE:
		return !m_oldMouseState.middleButton && m_nowMouseState.middleButton;
		break;
	case MOUSE_RIGHT:
		return !m_oldMouseState.rightButton && m_nowMouseState.rightButton;
		break;
	default:
		break;
	}

	return false;
}

bool MouseInput::GetMouse(int MouseCode)
{
	switch (MouseCode)
	{
	case MOUSE_LEFT:
		return m_nowMouseState.leftButton;
		break;
	case MOUSE_MIDDLE:
		return m_nowMouseState.middleButton;
		break;
	case MOUSE_RIGHT:
		return m_nowMouseState.rightButton;
		break;
	default:
		break;
	}

	return false;
}

bool MouseInput::GetMouseUp(int MouseCode)
{
	switch (MouseCode)
	{
	case MOUSE_LEFT:
		return m_oldMouseState.leftButton && !m_nowMouseState.leftButton;
		break;
	case MOUSE_MIDDLE:
		return m_oldMouseState.middleButton && !m_nowMouseState.middleButton;
		break;
	case MOUSE_RIGHT:
		return m_oldMouseState.rightButton && !m_nowMouseState.rightButton;
		break;
	default:
		break;
	}

	return false;
}

DirectX::SimpleMath::Vector2 MouseInput::GetMousePoint()
{
	int x = m_nowMouseState.x;
	int y = m_nowMouseState.y;

	return { static_cast<float>(x), static_cast<float>(y) };
}

DirectX::SimpleMath::Vector2 MouseInput::GetOldMousePoint()
{
	int x = m_oldMouseState.x;
	int y = m_oldMouseState.y;

	return { static_cast<float>(x), static_cast<float>(y) };
}

DirectX::SimpleMath::Vector2 MouseInput::GetScaledMousePoint()
{
	// 今の画面サイズを取得
	RECT rc;
	GetClientRect(ResourceManager::Instance().GetResources()->GetWindow(), &rc);
	
	int width  = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	// 割合を取得
	float ratioX = (float)Screen::WIDTH / width;
	float ratioY = (float)Screen::HEIGHT / height;

	return GetMousePoint() * DirectX::SimpleMath::Vector2{ ratioX, ratioY };
}

DirectX::SimpleMath::Vector2 MouseInput::GetMouseMoveValue()
{
	if (m_nowMouseState.positionMode == DirectX::Mouse::Mode::MODE_ABSOLUTE) 
	{
		return GetMousePoint() - GetOldMousePoint();
	}
	if (m_nowMouseState.positionMode == DirectX::Mouse::Mode::MODE_RELATIVE) 
	{
		int x = m_nowMouseState.x;
		int y = m_nowMouseState.y;

		return { static_cast<float>(x), static_cast<float>(y) };
	}

	return { 0, 0 };
}

int MouseInput::GetWheelMoveValue()
{
	return m_nowMouseState.scrollWheelValue - m_oldMouseState.scrollWheelValue;
}

void MouseInput::SetMode(DirectX::Mouse::Mode mode)
{
	DirectX::Mouse::Get().SetMode(mode);
}

void MouseInput::SetMousePoint(int x, int y)
{
	POINT pt = { x, y };
	HWND hwnd = ResourceManager::Instance().GetResources()->GetWindow();

	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void MouseInput::SetMouseVisible(bool visible)
{
	DirectX::Mouse::Get().SetVisible(visible);
}
