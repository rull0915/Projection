/**
 * @file    DepthCorrection.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/24
 *
 * @brief   2Dの際にZ軸の補正を行うコンポーネント
 */

#pragma once

#include "Components/World/WorldComponentBase.h"

/**
 * @brief 2Dの際にカメラ視点のZ軸方向を補正する
 */
class DepthCorrection : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:
	/** @brief 衝突時に補正を行う衝突法線のボーダー */
	static constexpr float CORRECTION_BORDER = 1.0f / 1.1415926535f;

public:

	DepthCorrection(RHEngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
	{
	}

	~DepthCorrection() = default;

	void OnCollisionEnter2D(RHEngine::HitContact2D& contact) override;
};
