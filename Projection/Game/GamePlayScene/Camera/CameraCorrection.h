/**
 * @file    CameraCorrection.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   カメラ補正コンポーネント
 */

#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "GameObject/GameObject.h"
#include "TPSCamera.h"

class RHEngine::Scene;

/**
 * @brief カメラがめり込まないように補正する
 */
class CameraCorrection : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	/** @brief カメラの本体 */
	TPSCamera* m_camera;

	/** @brief プレイヤーの参照 */
	RHEngine::Ref<RHEngine::GameObject> m_player;

	/** @brief プレイヤーのレイヤー */
	int m_playerLayer;

	/** @brief シーンポインタ */
	RHEngine::Scene* m_pScene;

public:

	CameraCorrection(RHEngine::IComponentOwner* own);
	~CameraCorrection() = default;

	void Start() override;

	void LateUpdate(const RHEngine::GameTimer& gameTimer) override;
};
