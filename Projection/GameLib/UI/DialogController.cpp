/**
 * @file    DialogController.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/30
 */

#include "pch.h"
#include "DialogController.h"

DialogController::DialogController(RHEngine::IComponentOwner* own)
	: RHEngine::BothComponentBase(own)
	, m_targetDialog{}
	, m_clickedOperation{ ClickedOperation::Close }
	, m_openId{ 0 }
	, m_closeId{ 0 }
{
	ADD_PROPERTY(DialogController, m_targetDialog);
	ADD_PROPERTY(DialogController, m_openButton);
	ADD_PROPERTY(DialogController, m_closeButton);
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
