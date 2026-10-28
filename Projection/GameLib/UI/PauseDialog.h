/**
 * @file    PauseDialog.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/30
 *
 * @brief   ポーズ画面のダイアログ
 */
#pragma once

#include "Dialog.h"
#include "Components/UI/RectTransform/RectTransform.h"

#include "GameLib/GameMath/EasingConfig.h"

/**
 * @brief ポーズ画面のダイアログ
 */
class PauseDialog : public Dialog
{
public:
	DECLARE_COMPONENT_TYPE(Dialog);

	// 拡大モード
	enum class Expansion
	{
		X, Y, XY
	};

private:
	// ダイアログのモード
	enum class Mode
	{
		Show, Hide, Showing, Hiding
	};

	// RectTransform
	RHEngine::RectTransform* m_rectTransform;

	// 使用するイージング
	EasingConfig m_openEasing;
	EasingConfig m_closeEasing;

	// 拡大モード
	Expansion m_expType;

	// 開閉にかける時間
	float m_transTime;

	// 初期状態のスケール
	DirectX::SimpleMath::Vector2 m_initScale;

	// 現在のモード
	Mode m_currentMode;

	// 今の経過時間
	float m_currentTime;

public:

	PauseDialog(RHEngine::IComponentOwner* own);
	~PauseDialog() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	// 開く
	void Show() override;

	// 閉じる
	void Hide() override;

private:
		
	// RectTransformのスケールを変更する関数
	void ChangeScale(float ratio);
};
