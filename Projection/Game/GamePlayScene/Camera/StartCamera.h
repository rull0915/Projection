/**
 * @file    StartCamera.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ゲーム開始時の演出を担当するカメラ
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "Components/World/Transform/Transform.h"

/**
 * @brief ゲーム開始時にステージ全体が見えるように動くカメラ
 */
class StartCamera : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	/** @brief 自身のトランスフォーム */
	RHEngine::Transform* m_pTransform;

	/** @brief ターゲットのトランスフォーム */
	RHEngine::Ref<RHEngine::Transform> m_targetTransform;

	/** @brief 目標座標 */
	DirectX::SimpleMath::Vector3 m_target;

	/** @brief 回る半径 */
	float m_radius;

	/** @brief 終了時の角度 */
	float m_finalAngle;

	/** @brief 回る各速度 */
	float m_angleVelocity;

	/** @brief 初期状態の高さ */
	float m_initialHeight;

	/** @brief 落ちる速さ */
	float m_fallVelocity;

	/** @brief 現在の角度 */
	float m_nowAngle;

	/** @brief オフセット */
	DirectX::SimpleMath::Vector3 m_offset;

public:

	StartCamera(RHEngine::IComponentOwner* own);
	~StartCamera() = default;

	void Awake() override;
	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
