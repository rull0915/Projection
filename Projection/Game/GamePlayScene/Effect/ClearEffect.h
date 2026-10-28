/**
 * @file    ClearEffect.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   クリア時の演出、シーン遷移を行うコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameLib/UI/Dialog.h"

/**
 * @brief クリア時にエフェクトを出しシーン遷移を行う
 */
class ClearEffect : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// クリアUIの名前
	std::string m_clearUIName;

	// クリアダイアログ
	RHEngine::Ref<Dialog> m_clearDialog;

	// クリアシーンへ移行するまでの時間
	float m_toClearTime;

	// 現在の時間
	float m_sumTime;

	// クリア時のタイムスケール
	float m_claerTimeScale;

public:

	ClearEffect(RHEngine::IComponentOwner* own);
	~ClearEffect() = default;

	void Awake() override;
	void Start() override;

	void OnDestroy() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
