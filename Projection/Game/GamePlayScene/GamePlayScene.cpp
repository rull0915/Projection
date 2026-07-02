#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

// コンポーネント
#include "Camera/ProjectionSmoothCamera.h"
#include "Camera/TPSCamera.h"
#include "Player/Player.h"
#include "Components/World/Sounds/AudioListener.h"
//#include "Components/World/Sounds/AudioSource.h"

// 管理クラス
#include "ChangeDimention/ChangeColliderComponent.h"
#include "System/ResourceManager.h"
#include "ObjectFactory.h"

// 入力
#include "Input/KeyInput.h"
#include "Input/MouseInput.h"

// その他
#include "Scene/Transition/FadeTransition.h"

#include "Saver/ObjectSaver.h"
#include "Loader/ObjectLoader.h"

#include "Common/EventBus.h"
#include "GamePlayEvent.h"

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
	ResourceManager::Instance().AddSound("Jump", L"Resources/Sounds/se_jump_006.wav");

	// モデルの読み込み
	ResourceManager::Instance().AddModel("Player", L"Resources/Models/Player.cmo");
	ResourceManager::Instance().AddModel("Goal", L"Resources/Models/Goal.cmo");
	ResourceManager::Instance().AddModel("Enemy", L"Resources/Models/Enemy.cmo");

	// マウスを相対モードに
	Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);

	// 次元管理クラスの初期化
	m_dimentionManager.Initialize();

	// 敵管理クラスの初期化
	m_enemyManager.Initialize();

	//======== オブジェクトの追加 ========//
	// カメラ
	m_camera = Generate();

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	SetMainCamera(cameraComponent);

	// リスナーに設定

	// プレイヤーを生成
	m_player = ObjectFactory::CreatePlayer(this, { 0, -2, 0 });
	//m_player = Generate();
	m_player->AddComponent<AudioListener>();

	// プレイヤーをテスト保存
	//ObjectLoader::LoadFromFile(L"Resources/Objects/Player.gameobject", m_player);
	ObjectSaver::SaveToFile(L"Resources/Objects/Player.gameobject", m_player);

	// カメラのターゲットに設定
	m_camera->AddComponent<TPSCamera>()->SetTarget(m_player->GetComponent<Transform>());

	// 敵を生成
	m_enemy = Generate();
	//ObjectLoader::LoadFromFile(L"Resources/Objects/Enemy2.gameobject", m_enemy);

	// ゴールの生成
	ObjectFactory::CreateGoal(this, { 0, 10.0f, 0 });

	// 地面を生成
	ObjectFactory::CreateCube(this, { 0, -3, 0 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 3, 0, -10 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 2, -1.5, -5 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 0, 4, 5 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 0, 7, 3 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 1, 3, -10 }, { 0, 0, 0 }, { 1, 6, 3 });

	// 次元管理クラスにカメラを渡す
	m_dimentionManager.SetCamera(m_camera->GetComponent<ProjectionSmoothCamera>());

	// 敵管理クラスにプレイヤーを渡す
	m_enemyManager.SetPlayer(m_player->GetComponent<Transform>());

	// ゴールイベントの追加
	EventBus<GamePlayEvent>::Register(
		GamePlayEvent::Goal,
		[this]()
		{
			ChangeScene("Title");
		}
	);
}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	// 警告潰し
	gameTimer;

	auto camera = m_camera->GetComponent<Transform>();

	// 次元管理クラスの更新
	m_dimentionManager.Update();

	// 敵管理クラスの更新
	m_enemyManager.Update(gameTimer);

	// Qキーで次元の変更
	if (Input::Key::Get(Input::State::Down, Input::Key::Code::Q))
	{
		TryChangeDimention();
	}

	// Rキーでリトライ
	if (Input::Key::Get(Input::State::Down, Input::Key::Code::R))
	{
		ChangeScene(
			"GamePlay",
			std::make_unique<Transition::Fade>(0.5f),
			std::make_unique<Transition::Fade>(0.5f)
		);
	}
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
	m_enemyManager.DebugRenderer(renderer);
}

// 終了関数
void GamePlayScene::Finalize()
{
}

// コンポーネントが追加されたときに呼ばれる関数
void GamePlayScene::RegisterComponentOnDerived(ComponentBase* component)
{
	// 次元変更マネージャーに通知
	if (component->GetID() == TypeIDGenerator::GetID<ChangeColliderComponent>())
	{
		m_dimentionManager.AddChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == TypeIDGenerator::GetID<LandingCandidatePoints>())
	{
		m_enemyManager.AddPoints(static_cast<LandingCandidatePoints*>(component));
	}
	if (component->GetID() == TypeIDGenerator::GetID<LandingCandidatePoints2D>())
	{
		m_enemyManager.AddPoints(static_cast<LandingCandidatePoints2D*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == TypeIDGenerator::GetID<Enemy>())
	{
		m_enemyManager.AddEnemy(static_cast<Enemy*>(component));
	}
}

// コンポーネントが削除されるときに呼ばれる関数
void GamePlayScene::UnRegisterComponentOnDerived(ComponentBase* component)
{
	// 次元変更マネージャーに通知
	if (component->GetID() == TypeIDGenerator::GetID<ChangeColliderComponent>())
	{
		m_dimentionManager.RemoveChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == TypeIDGenerator::GetID<LandingCandidatePoints>())
	{
		m_enemyManager.RemovePoints(static_cast<LandingCandidatePoints*>(component));
	}
	if (component->GetID() == TypeIDGenerator::GetID<LandingCandidatePoints2D>())
	{
		m_enemyManager.RemovePoints(static_cast<LandingCandidatePoints2D*>(component));
	}
	// 敵管理クラスに通知
	if (component->GetID() == TypeIDGenerator::GetID<Enemy>())
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

	// 敵の切り替え
	m_enemyManager.ChangeDimantion();
}
