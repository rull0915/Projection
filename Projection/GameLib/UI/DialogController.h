/**
 * @file    DialogController.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/30
 *
 * @brief   ダイアログコンポーネントを操作するコンポーネント
 */
#pragma once

#include "Components/Both/BothComponentBase.h"
#include "Dialog.h"
#include "Components/UI/Behaviour/ButtonUI.h"

/**
 * @brief ダイアログを操作する
 */
class DialogController : public RHEngine::BothComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(BothComponentBase);

	// クリック時の操作
	enum class ClickedOperation
	{
		Open, Close
	};

private:

	// 操作対象のDialog
	RHEngine::Ref<Dialog> m_targetDialog;

	// クリック時の操作
	ClickedOperation m_clickedOperation;

	// 開くボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_openButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_closeButton;

	// 削除用ID
	int m_openId;
	int m_closeId;

public:

	DialogController(RHEngine::IComponentOwner* own);
	~DialogController() = default;

	void Start() override;

	void OnDestroy() override;
};
