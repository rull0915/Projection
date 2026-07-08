#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

// コンポーネント
#include "Camera/ProjectionSmoothCamera.h"
#include "Camera/TPSCamera.h"
#include "Player/Player.h"
#include "Components/World/Sounds/AudioListener.h"
#include "Components/World/Renderer/Skybox/SkyboxComponent.h"

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
	, m_renderTarget{ std::make_unique<RenderTarget>() }
{
}

GamePlayScene::~GamePlayScene()
{
}

// 初期化関数
void GamePlayScene::Initialize()
{
	// 初期化
	m_renderTarget->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		1280, 720
	);

	// マウスを相対モードに
	Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);

	// 次元管理オブジェクトを生成
	auto* manager = Generate();
	m_dimentionManager = manager->AddComponent<DimentionManager>();
	m_enemyManager = manager->AddComponent<EnemyManager>();

	//======== オブジェクトの追加 ========//

	m_birdCamera = Generate({ 0, 20, 0 });
	m_birdCamera->AddComponent<StandardCamera>();
	m_birdCamera->GetComponent<Transform>()->SetLocalEulerAngle({ DirectX::XMConvertToRadians(-90), 0, 0 });
	 
	// カメラ
	m_camera = Generate();

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();

	// リスナーに設定

	// プレイヤーを生成
	m_player = ObjectFactory::CreatePlayer(this, { 0, -2, 0 });

	// カメラのターゲットに設定
	m_camera->AddComponent<TPSCamera>()->SetTarget("Player");
	m_player->AddComponent<SkyboxComponent>()->SetTexture("Skybox");

	// 敵を生成
	m_enemy = ObjectFactory::CreateEnemy(this, { 3, 2, -10 });

	// ゴールの生成
	ObjectFactory::CreateGoal(this, { 0, 10.0f, 0 });

	// 地面を生成
	ObjectFactory::CreateCube(this, { 0, -3, 0 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 3, 0, -10 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 2, -1.5, -5 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 0, 4, 5 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 0, 7, 3 }, { 0, 0, 0 }, { 3, 1, 3 });
	ObjectFactory::CreateCube(this, { 1, 3, -10 }, { 0, 0, 0 }, { 1, 6, 3 });

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
	RenderWithContext(
		{
			m_birdCamera->GetComponent<StandardCamera>(),
			m_renderTarget.get(),
			{ 0, 0, 0, 0 },
			DrawFlag::World
		},
		renderer
	);
}

void GamePlayScene::RenderOnScreen(Renderer& renderer)
{
	renderer.Draw().Sprite()
		.Rect({ 80, 40 }, { 400, 220 })
		.Execute(m_renderTarget->GetShaderResourceView());
}

// 終了関数
void GamePlayScene::Finalize()
{
	ObjectSaver::SaveSceneToFile(L"Resources/Scenes/GamePlayScene.scene", this);
}

// 次元の切り替えを行う関数
void GamePlayScene::TryChangeDimention()
{
	// 切り替え中なら何もしない
	if (m_dimentionManager->IsChanging()) return;

	// 2D->3D
	if (m_dimentionManager->GetIs2D())
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
	m_dimentionManager->ChangeDimention();

	// プレイヤーの切り替え
	m_player->GetComponent<Player>()->ChangeDimention();

	// 敵の切り替え
	m_enemyManager->ChangeDimantion();
}
