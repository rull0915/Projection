#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

// コンポーネント
#include "Camera/ProjectionSmoothCamera.h"
#include "Camera/TPSCamera.h"
#include "Player/Player.h"
#include "Stage/Components/MoveComponent.h"

// 管理クラス
#include "ChangeDimention/ChangeColliderComponent.h"
#include "ObjectFactory.h"

// 入力
#include "GameLib/Input/KeyInput.h"

// その他
#include <GameLib/Random.h>

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
	, m_player{ nullptr }
{
}

GamePlayScene::~GamePlayScene()
{
}

// 初期化関数
void GamePlayScene::Initialize()
{
	m_dimentionManager.Initialize();

	m_enemyManager.Initialize();
	// オブジェクトの追加
	// カメラ
	m_camera = Generate();

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	GetCamera().SetMainCamera(cameraComponent);

	// プレイヤーを生成
	m_player = ObjectFactory::CreatePlayer(this);

	// カメラのターゲットに設定
	m_camera->AddComponent<TPSCamera>()->SetTarget(m_player->GetComponent<Transform>());

	ObjectFactory::CreateCube(this, { 0, -3, 0 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 0, -3, -10 }, { 0, 0, 0 }, { 3, 1, 3 });

	for (int i = 0; i < 10; i++)
	{
		auto cube = ObjectFactory::CreateCube(this, 
			{ Random::GetFloat(-10, 10), Random::GetFloat(-3, 5), Random::GetFloat(-10, 10) }
			, { 0, 0, 0 }
			,{ 3.0f, 1.0f, 3.0f } 
		);
		if (i == 0) cube->AddComponent<MoveComponent>()->SetFunc(TestMove);
	}

	// 次元管理クラスにカメラを渡す
	m_dimentionManager.SetCamera(m_camera->GetComponent<ProjectionSmoothCamera>());

	// 敵管理クラスにプレイヤーを渡す
	m_enemyManager.SetPlayer(m_player->GetComponent<Transform>());

	// 敵のテスト
	auto enemy = ObjectFactory::CreateEnemy(this);

}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	m_dimentionManager.Update();

	m_enemyManager.Update(gameTimer);

	if (KeyInput::GetKeyDown(KeyCode::Q))
	{
		TryChangeDimention();
	}

	if (KeyInput::GetKeyDown(KeyCode::R))
	{
		ChangeScene(
			"GamePlay",
			std::make_unique<FadeTransition>(0.5f, Transition::Mode::In),
			std::make_unique<FadeTransition>(0.5f, Transition::Mode::Out)
		);
	}
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
}

// 終了関数
void GamePlayScene::Finalize()
{
}

// コンポーネントが追加されたときに呼ばれる関数
void GamePlayScene::RegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーに通知
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.AddChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == ComponentID::LandingCandidatePoints)
	{
		m_enemyManager.AddPoints(static_cast<LandingCandidatePoints*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == ComponentID::Enemy)
	{
		m_enemyManager.AddEnemy(static_cast<Enemy*>(component));
	}
}

// コンポーネントが削除されるときに呼ばれる関数
void GamePlayScene::UnRegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーに通知
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.RemoveChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == ComponentID::LandingCandidatePoints)
	{
		m_enemyManager.RemovePoints(static_cast<LandingCandidatePoints*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == ComponentID::Enemy)
	{
		m_enemyManager.RemoveEnemy(static_cast<Enemy*>(component));
	}
}

// 次元の切り替えを行う関数
void GamePlayScene::TryChangeDimention()
{
	// 切り替え中なら何もしない
	if (m_dimentionManager.IsChanging()) return;

	// 2D->3D
	if (m_dimentionManager.GetIs2D())
	{
		// TPSをアクティブ化
		m_camera->GetComponent<TPSCamera>()->SetActive(true);

		// カメラの親子関係を解除
		m_camera->GetComponent<Transform>()->SetParent(nullptr);
	}
	else
	{
		// TPSカメラを非アクティブ化
		m_camera->GetComponent<TPSCamera>()->SetActive(false);

		// カメラをプレイヤーの子に設定
		m_camera->GetComponent<Transform>()->SetParent(m_player->GetComponent<Transform>());
	}

	// 次元の切り替え
	m_dimentionManager.ChangeDimention();

	// プレイヤーの切り替え
	m_player->GetComponent<Player>()->ChangeDimention();
}
