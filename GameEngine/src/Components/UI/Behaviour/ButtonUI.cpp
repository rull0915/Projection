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

#include "Components/UI/Graphics/ImageUI.h"

namespace REngine
{
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
	{
		ADD_PROPERTY(m_normalColor);
		ADD_PROPERTY(m_hoverdColor);
		ADD_PROPERTY(m_pressedColor);
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
			// クリック時イベントを実行
			for (auto& event : m_onClicked)
			{
				event.second();
			}
		}

		// フラグリセット
		m_isPressed = false;

		// Imageの色を変更
		if (auto* image = GetOwn()->GetComponent<ImageUI>())
		{
			image->SetMulColor(m_normalColor);
		}
	}

	int ButtonUI::AddOnClicked(OnClickEvent event)
	{
		// 全体で一つのトークンを共有
		static EventToken token = 0;

		// トークンをインクリメント
		token++;

		// イベントを追加
		m_onClicked.insert({ token, event });

		// トークンを返す
		return token;
	}

	void ButtonUI::RemoveOnClicked(EventToken token)
	{
		// リストから削除
		m_onClicked.erase(token);
	}

	void ButtonUI::Submit()
	{
		// クリック時イベントを実行
		for (auto& event : m_onClicked)
		{
			event.second();
		}
	}
}	// namespace REngine
