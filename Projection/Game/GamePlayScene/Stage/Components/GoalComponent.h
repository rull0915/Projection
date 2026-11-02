/**
 * @file    GoalComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ゴールコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

/**
 * @brief ゴールコンポーネント
 */
class GoalComponent : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

public:

	GoalComponent(RHEngine::IComponentOwner* own);
	~GoalComponent() = default;

	void OnTriggerEnter(RHEngine::HitContact& contact) override;
};
