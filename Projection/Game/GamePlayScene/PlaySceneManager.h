/**
 * @file    PlaySceneManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ゲームプレイシーンの管理コンポーネント
 */

#pragma once

#include <vector>

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "ChangeDimention/DimentionManager.h"
#include "GameObject/GameObject.h"

#include "Common/EventBus.h"
#include "GamePlayEvent.h"

class Canvas;

class PlaySceneManager : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// カメラ
	RHEngine::Ref<RHEngine::GameObject> m_camera;

	// プレイヤー
	RHEngine::Ref<RHEngine::GameObject> m_player;

	// 次元管理
	RHEngine::Ref<DimentionManager> m_dimentionManager;

	// イベントのID
	std::vector<RHEngine::EventBus<GamePlayEvent>::Token> m_eventIds;

public:

	PlaySceneManager(RHEngine::IComponentOwner* own);
	~PlaySceneManager() = default;

	void Awake() override;

	void OnDestroy() override;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

private:

	// 次元の切り替えを試みる関数
	void TryChangeDimention();
};
