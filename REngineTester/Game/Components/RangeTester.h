/**
 * @file    RangeTester.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 *
 * @brief   範囲プロパティのテストコンポーネント
 */

#pragma once

#include "Components/World/WorldComponentBase.h"

/**
 * @brief 範囲プロパティのテスト
 */
class RangeTester : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(RangeTester);

private:

	RHEngine::Range m_range;

public:

	RangeTester(RHEngine::IComponentOwner* own);
	~RangeTester() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
