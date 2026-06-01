//====================================================//
// ファイル名  : ButtonUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要       : ボタンUIクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ButtonUI.h"

#include "GameLib/Input/MouseInput.h"
#include "../../../UIObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI(UIObject* own)
	: UIComponent(own)
	, m_normalColor{ 0xFFFFFF }
	, m_hoverdColor{ 0xF5F5F5 }
	, m_pressedColor{ 0xC8C8C8 }
	, m_isHovered{ false }
	, m_isPressed{ false }
	, m_onClick{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
ButtonUI::~ButtonUI()
{}

/// <summary>
/// 更新処理
/// </summary>
void ButtonUI::Update(float elapsedTime)
{
	// マウスの状態を調べる
	elapsedTime;

	// 押されたとき
	if (MouseInput::GetMouseDown(MOUSE_LEFT))
	{
		// 自分にマウスが乗っているなら
		if (m_isHovered)
		{
			// 押下フラグをONに
			m_isPressed = true;

			// Imageの色を変更
			if (auto* image = GetOwn()->GetComponent<ImageUI>())
			{
				image->SetMulColor(m_pressedColor);
			}
		}
	}
	// 離されたとき
	else if (MouseInput::GetMouseUp(MOUSE_LEFT))
	{
		// 自分にマウスが乗っているなら
		if (m_isHovered)
		{
			// 関数を実行
			if(m_onClick) m_onClick();
		}

		// フラグリセット
		m_isPressed = false;

		// Imageの色を変更
		if (auto* image = GetOwn()->GetComponent<ImageUI>())
		{
			image->SetMulColor(m_normalColor);
		}
	}
	// どちらでもないとき
	else
	{
		// Imageの色を変更
		if (auto* image = GetOwn()->GetComponent<ImageUI>())
		{
			if(!m_isPressed) image->SetMulColor(m_isHovered ? m_hoverdColor : m_normalColor);
		}
	}

	// フラグをリセットする
	m_isHovered = false;
}
