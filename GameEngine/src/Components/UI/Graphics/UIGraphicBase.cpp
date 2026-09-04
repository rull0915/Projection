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

namespace REngine
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIGraphicBase::UIGraphicBase(IComponentOwner* owner)
		: UIComponentBase(owner)
		, m_pRectTransform{ owner->GetComponent<RectTransform>() }
		, m_color{ 1, 1, 1, 1 }
		, m_mulColor{ 1, 1, 1, 1 }
	{
		ADD_PROPERTY(UIGraphicBase, m_color);
		ADD_PROPERTY(UIGraphicBase, m_mulColor);
	}
}	// namespace REngine
