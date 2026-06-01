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
#include "BaseGraphic.h"

#include "../../UIObject.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
BaseGraphic::BaseGraphic(UIObject* own, UIComponentID ID, bool isMain)
    : BaseUIComponent(own, ID, isMain)
    , m_pRectTransform{ own->GetComponent<RectTransform>() }
    , m_color{ 0xFFFFFF }
    , m_mulColor{ 0xFFFFFF }
    , m_alpha{ 1.0f }
{
}
