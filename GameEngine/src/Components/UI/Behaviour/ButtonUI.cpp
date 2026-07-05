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
	: UIBehaviorBase(owner)
	, m_normalColor{ 1.0f, 1.0f, 1.0f, 0 }
	, m_hoverdColor{ 0.92f, 0.92f, 0.92f, 0 }
	, m_pressedColor{ 0.75f, 0.75f, 0.75f }
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
	// 警告対策
	gameTimer;

	if (!m_isPressed) 
	{
		// Imageの色を変更
		if (auto* image = GetOwn()->GetComponent<ImageUI>())
		{
			image->SetMulColor(IsHovered() ? m_hoverdColor : m_normalColor);
		}
	}

	// フラグをリセットする
	SetHovered(false);
}

void ButtonUI::OnMouseDown()
{
	// 自分にマウスが乗っているなら
	if (IsHovered())
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

void ButtonUI::OnMouseUp()
{
	// 押されていたら
	if (m_isPressed && IsHovered())
	{
		// 関数を実行
		if (m_onClick) m_onClick();
	}

	// フラグリセット
	m_isPressed = false;

	// Imageの色を変更
	if (auto* image = GetOwn()->GetComponent<ImageUI>())
	{
		image->SetMulColor(m_normalColor);
	}
}
