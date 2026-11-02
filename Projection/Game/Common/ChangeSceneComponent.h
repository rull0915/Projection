/**
 * @file    ChangeSceneComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/02
 *
 * @brief   シーンを変更するコンポーネント
 */

#pragma once

#include "Components/Both/BothComponentBase.h"
#include "Components/UI/Behaviour/ButtonUI.h"
#include "GameObject/GameObject.h"

/**
 * @brief シーンを変更するコンポーネント
 */
class ChangeSceneComponent : public RHEngine::BothComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(BothComponentBase);

private:

	/** @brief シーンを変更するボタンの参照 */
	RHEngine::Ref<RHEngine::ButtonUI> m_button;

	/** @brief コントローラー操作を表示するUI */
	RHEngine::Ref<RHEngine::GameObject> m_padUI;

	/** @brief 遷移するシーン名 */
	std::string m_targetScene;

	/** @brief イベントトークン (ボタンから押下時イベントを削除する際に使用) */
	int m_eventToken;

public:

	ChangeSceneComponent(RHEngine::IComponentOwner* own);
	~ChangeSceneComponent() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& timer) override;

	void OnDestroy() override;
};
