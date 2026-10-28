/**
 * @file    SwingComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   UIを揺らすコンポーネント
 */

#pragma once

#include "Components/UI/UIComponentBase.h"
#include "Components/UI/RectTransform/RectTransform.h"

/**
 * @brief UIをSin波で揺らす
 */
class SwingComponent : public RHEngine::UIComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(UIComponentBase);

private:

	/** @brief 自身のRectTransform */
	RHEngine::RectTransform* m_rectTransform;

	// 揺れの幅

public:

	SwingComponent(RHEngine::IComponentOwner* pOwner);
	~SwingComponent() = default;

	void Awake() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
