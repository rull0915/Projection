/**
 * @file    SelectBlock.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ステージ選択ブロックのコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameLib/UI/Dialog.h"
#include "Components/UI/Graphics/TextUI.h"

/**
 * @brief ステージ選択ブロック
 */
class SelectBlock : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// ターゲットシーン名
	std::string m_targetScene;

	// ターゲットダイアログ
	RHEngine::Ref<Dialog> m_targetDialog;

	// ステージ名テキスト
	RHEngine::Ref<RHEngine::TextUI> m_nameText;

public:

	SelectBlock(RHEngine::IComponentOwner* own);
	~SelectBlock() = default;

	void Start() override;

	void OnTriggerStay(RHEngine::HitContact& contact) override;
	void OnTriggerEnter(RHEngine::HitContact& contact) override;
	void OnTriggerExit(RHEngine::HitContact& contact) override;
};
