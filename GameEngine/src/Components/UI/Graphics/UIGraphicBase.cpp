//====================================================//
// ファイル名  : BaseComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/25
//
// 概要       : グラフィックコンポーネントの基底クラス  
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/UI/RectTransform/RectTransform.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
UIGraphicBase::UIGraphicBase(IComponentOwner* owner)
	: UIComponentBase(owner)
	, m_pRectTransform{ owner->GetComponent<RectTransform>() }
	, m_color{ 0xFFFFFF }
	, m_mulColor{ 0xFFFFFF }
	, m_alpha{ 1.0f }
{
	ADD_PROPERTY(m_color);
	ADD_PROPERTY(m_mulColor);
	ADD_PROPERTY(m_alpha);
}
