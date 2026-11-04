/**
 * @file    BaseComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/25
 */

#include "pch.h"

#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/UI/RectTransform/RectTransform.h"

namespace RHEngine
{
	UIGraphicBase::UIGraphicBase(IComponentOwner* pOwner)
		: UIComponentBase(pOwner)
		, m_pRectTransform{ pOwner->GetComponent<RectTransform>() }
		, m_color{ 1, 1, 1, 1 }
		, m_mulColor{ 1, 1, 1, 1 }
	{
		ADD_PROPERTY(UIGraphicBase, m_color);
		ADD_PROPERTY(UIGraphicBase, m_mulColor);
	}
}	// namespace RHEngine
