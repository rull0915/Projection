//====================================================//
// ファイル名  : GameOverComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/13
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameOverComponent.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"

#include "Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
GameOverComponent::GameOverComponent(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_overSceneName{ "GameOver" }
	, m_playerName{}
	, m_player{ nullptr }
	, m_playerComponent{ nullptr }
	, m_border3D{ -10.0f }
	, m_border2D{ -10.0f }
{
	ADD_PROPERTY(m_overSceneName);
	ADD_PROPERTY(m_playerName);
	ADD_PROPERTY(m_border2D);
	ADD_PROPERTY(m_border3D);
}

// 生成直後に一度呼ばれます
void GameOverComponent::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void GameOverComponent::Start()
{
	// シーンを取得
	Scene* scene = static_cast<GameObject*>(GetOwn())->GetScene();

	// プレイヤーを取得
	m_player = scene->GetObjectFinder()->FindWithNameInWorld(m_playerName);

	// プレイヤーコンポーネントを取得
	m_playerComponent = m_player->GetComponent<Player>();
}

// 毎フレーム呼ばれます
void GameOverComponent::Update(const GameTimer & gameTimer)
{
	// 2Dなら
	if (m_playerComponent->Is2D())
	{
		// 高さ
		DirectX::SimpleMath::Vector2 pos = WorldSetting2D::Instance().World3DToLocal2D(m_player->GetComponent<Transform>()->GetWorldPosition());

		// ボーダー以下なら
		if (pos.y < m_border2D)
		{
			// ゲームオーバー
			SceneManager::Instance().RequestSceneChange(m_overSceneName, std::make_unique<Transition::Fade>(0.3f), std::make_unique<Transition::Fade>(0.3f));
		}
	}
	// 3Dなら
	else
	{
		// 位置
		DirectX::SimpleMath::Vector3 pos = m_player->GetComponent<Transform>()->GetWorldPosition();

		// ボーダー以下なら
		if (pos.y < m_border3D)
		{
			// ゲームオーバー
			SceneManager::Instance().RequestSceneChange(m_overSceneName, std::make_unique<Transition::Fade>(0.3f), std::make_unique<Transition::Fade>(0.3f));
		}
	}
}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void GameOverComponent::LateUpdate(const GameTimer & gameTimer)
{}
