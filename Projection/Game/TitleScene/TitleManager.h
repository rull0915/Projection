/**
 * @file    TitleManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/01
 *
 * @brief   タイトルシーンの管理
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "Components/UI/Behaviour/ButtonUI.h"
#include "GameObject/GameObject.h"
#include "GameLib/UI/Dialog.h"
#include "Components/Both/Sounds/AudioSource.h"

/**
 * @brief タイトルシーンを管理する
 */
class TitleManager : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// 遷移対象のシーン名
	std::string m_targetScene;

	// プレイボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_playButton;

	// 終了ボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_exitButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_exitAcceptButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_exitCancelButton;

	// 追加したイベント
	RHEngine::ButtonUI::EventToken m_playEventToken;
	RHEngine::ButtonUI::EventToken m_exitEventToken;

	// コントローラーが接続されているときのみ表示されるUI
	RHEngine::Ref<RHEngine::GameObject> m_padUI;
	RHEngine::Ref<RHEngine::GameObject> m_padUIOnDialog;

	// 終了ダイアログ
	RHEngine::Ref<Dialog> m_quitDialog;

	// 音楽
	RHEngine::Ref<RHEngine::AudioSource> m_audioSource;

public:

	TitleManager(RHEngine::IComponentOwner* own);
	~TitleManager() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	void OnDestroy() override;
};
