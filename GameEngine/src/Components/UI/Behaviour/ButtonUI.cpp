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
#include "Components/UI/Behaviour/ButtonUI.h"

#include "Input/MouseInput.h"
#include "Components/UI/Graphics/ImageUI.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI(IComponentOwner* owner)
	: UIComponentBase(owner)
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
void ButtonUI::Update(const GameTimer& gameTimer)
{
	// マウスの状態を調べる
	gameTimer;

	// 押されたとき
	if (Input::Mouse::Get(Input::State::Down, Input::Mouse::Button::Left))
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
	else if (Input::Mouse::Get(Input::State::Up, Input::Mouse::Button::Left))
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
