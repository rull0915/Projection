/**
 * @file    GameOverComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ゲームオーバーコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameObject/GameObject.h"
#include "Player/Player.h"

/**
 * @brief ゲームオーバーコンポーネント
 */
class GameOverComponent : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// 移行先のシーン名
	std::string m_overSceneName;

	// プレイヤー名
	std::string m_playerName;

	// プレイヤー
	RHEngine::GameObject* m_player;

	// プレイヤーコンポーネント
	Player* m_playerComponent;

	// ゲームオーバーボーダー
	float m_border3D;
	float m_border2D;

public:

	GameOverComponent(RHEngine::IComponentOwner* own);
	~GameOverComponent() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
