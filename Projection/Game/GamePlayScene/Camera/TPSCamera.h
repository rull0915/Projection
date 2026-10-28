/**
 * @file    TPSCamera.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/07
 *
 * @brief   カメラをTPS用に動かすコンポーネント
 */

#pragma once

#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

/**
 * @brief TPSカメラ
 */
class TPSCamera : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	/** @brief マウス感度 */
	float m_sensitivity;

	/** @brief ターゲットとの間隔 */
	float m_distance;

	/** @brief 自分のトランスフォーム */
	RHEngine::Transform* m_pOwnTransform;

	/** @brief 注視点とするトランスフォーム */
	RHEngine::Ref<RHEngine::Transform> m_pTargetTransform;

	/** @brief 角度 */
	DirectX::SimpleMath::Vector2 m_angle;

	/** @brief 現在のカメラ位置 */
	DirectX::SimpleMath::Vector3 m_position;

public:

	TPSCamera(RHEngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_pOwnTransform{ nullptr }
		, m_pTargetTransform{}
		, m_angle{ 0.0f, 0.0f }
		, m_sensitivity{ 0.5f }
		, m_distance{ 5.0f }
		, m_position{ 0, 0, 0 }
	{
		ADD_PROPERTY(TPSCamera, m_pTargetTransform);
		ADD_PROPERTY(TPSCamera, m_sensitivity);
		ADD_PROPERTY(TPSCamera, m_distance);
	}

	~TPSCamera() = default;

	void Awake() override;

	void OnEnable() override;
	void OnDisable() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	/**
	 * @brief 理想位置を取得する
	 */
	DirectX::SimpleMath::Vector3 GetIdealPosition() const { return m_position; }

	/**
	 * @brief 回転を取得する
	 */
	DirectX::SimpleMath::Vector2 GetRotation() const { return m_angle; }
};
