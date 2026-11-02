/**
 * @file    EasingTestComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/21
 *
 * @brief   イージングのテストをするコンポーネント
 */
#pragma once

#define IS_COMPONENT(EasingTestComponent)

#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

#include "EasingConfig.h"
#include "Components/Interface/IDebugRenderable.h"

/**
 * @brief イージングをテストする
 */
class EasingTestComponent : public RHEngine::WorldComponentBase, public RHEngine::IDebugRenderable
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// トランスフォームポインタ
	RHEngine::Transform* m_pTransform;

	// 開始点
	DirectX::SimpleMath::Vector3 m_start;

	// 終了点
	DirectX::SimpleMath::Vector3 m_end;

	// 再生倍率
	float m_playScale;

	// イージング情報
	EasingConfig m_easingConfig;

	// 経過時間の合計
	float m_sumTime;

	// ループフラグ
	bool m_loop;

	// 次のループまでの間隔
	float m_loopInterval;

public:

	EasingTestComponent(RHEngine::IComponentOwner* own);
	~EasingTestComponent() = default;

	void Awake() override;
	void OnEnable() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	// デバッグ描画関数
	void DebugRender(RHEngine::Renderer& renderer, const DirectX::SimpleMath::Color& color) override;
};
