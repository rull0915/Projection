//====================================================//
// ファイル名  : DialogController.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/30
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DialogController.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
DialogController::DialogController(REngine::IComponentOwner* own)
	: REngine::BothComponentBase(own)
	, m_targetDialog{}
	, m_clickedOperation{ ClickedOperation::Close }
	, m_openId{ 0 }
	, m_closeId{ 0 }
{
	ADD_PROPERTY(m_targetDialog);
	ADD_PROPERTY(m_openButton);
	ADD_PROPERTY(m_closeButton);
}

void DialogController::Start()
{
	// ボタンクリック時の関数を追加
	if (m_openButton)
	{
		m_openId = m_openButton->AddOnClicked([this]() { m_targetDialog->Show(); });
	}
	if (m_closeButton)
	{
		m_closeId = m_closeButton->AddOnClicked([this]() { m_targetDialog->Hide(); });
	}
}

void DialogController::OnDestroy()
{
	// イベントを削除
	if (m_openButton)
	{
		m_openButton->RemoveOnClicked(m_openId);
	}
	if (m_closeButton)
	{
		m_closeButton->RemoveOnClicked(m_closeId);
	}
}
