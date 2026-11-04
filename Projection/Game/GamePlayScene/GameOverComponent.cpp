/**
 * @file    GameOverComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/13
 */

#include "pch.h"
#include "GameOverComponent.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/ObjectFinder.h"
#include "Scene/Transition/FadeTransition.h"

#include "Settings/WorldSetting2D.h"

GameOverComponent::GameOverComponent(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_overSceneName{ "GameOver" }
	, m_playerName{}
	, m_player{ nullptr }
	, m_playerComponent{ nullptr }
	, m_border3D{ -10.0f }
	, m_border2D{ -10.0f }
{
	ADD_PROPERTY(GameOverComponent, m_overSceneName);
	ADD_PROPERTY(GameOverComponent, m_playerName);
	ADD_PROPERTY(GameOverComponent, m_border2D);
	ADD_PROPERTY(GameOverComponent, m_border3D);
}

void GameOverComponent::Start()
{
	// シーンを取得
	RHEngine::Scene* scene = static_cast<RHEngine::GameObject*>(GetOwn())->GetScene();

	// プレイヤーを取得
	m_player = scene->GetObjectFinder()->FindWithName(m_playerName);

	// プレイヤーコンポーネントを取得
	m_playerComponent = m_player->GetComponent<Player>();
}

void GameOverComponent::Update(const RHEngine::GameTimer & gameTimer)
{
	gameTimer;

	// 2Dなら
	if (m_playerComponent->Is2D())
	{
		// 高さ
		DirectX::SimpleMath::Vector2 pos = RHEngine::WorldSetting2D::Instance().World3DToLocal2D(m_player->GetComponent<RHEngine::Transform>()->GetWorldPosition());

		// ボーダー以下なら
		if (pos.y < m_border2D)
		{
			// ゲームオーバー
			RHEngine::SceneManager::Instance().RequestSceneChange(m_overSceneName, std::make_unique<RHEngine::Transition::Fade>(0.3f), std::make_unique<RHEngine::Transition::Fade>(0.3f));
		}
	}
	// 3Dなら
	else
	{
		// 位置
		DirectX::SimpleMath::Vector3 pos = m_player->GetComponent<RHEngine::Transform>()->GetWorldPosition();

		// ボーダー以下なら
		if (pos.y < m_border3D)
		{
			// ゲームオーバー
			RHEngine::SceneManager::Instance().RequestSceneChange(m_overSceneName, std::make_unique<RHEngine::Transition::Fade>(0.3f), std::make_unique<RHEngine::Transition::Fade>(0.3f));
		}
	}
}
