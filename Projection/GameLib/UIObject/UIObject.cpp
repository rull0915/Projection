//====================================================//
// ファイル名  : BaseComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要       : コンポーネントの基底クラス  
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "UIObject.h"

#include "Canvas/Canvas.h"

//====================================================//
// 関数の実体宣言
//====================================================//

UIObject::UIObject()
	: m_pTransform{ std::make_unique<RectTransform>(this) }
	, m_isDead{ false }
	, m_isActive{ true }
{
}

Canvas* UIObject::GetCanvas()
{
	// 自身の所属するツリーを辿ってCanvasを探す
    RectTransform* current = this->m_pTransform.get();
    Canvas* canvas = nullptr;

    while (current != nullptr)
    {
        canvas = dynamic_cast<Canvas*>(current->GetOwn());

        // Canvasならループ終了
        if (canvas) break;

        // currentを親に
        current = current->GetParent();
    }

    return canvas;
}
