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
	: REngine::ButtonListenerBase(own)
	, m_targetDialog{}
	, m_clickedOperation{ ClickedOperation::Close }
{
	ADD_PROPERTY(m_targetDialog);
	ADD_PROPERTY(m_clickedOperation);
}

void DialogController::OnClicked()
{
	switch (m_clickedOperation)
	{
	case DialogController::ClickedOperation::Open:

		if (m_targetDialog) m_targetDialog->Show();

		break;
	case DialogController::ClickedOperation::Close:

		if (m_targetDialog) m_targetDialog->Hide();

		break;
	default:
		break;
	}
}
