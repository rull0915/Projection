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
#include "Enemy/AI/AStarPathFinder.h"
#include "GameLib/GameObject/Settings/TimeSettings.h"

// 入力
#include "GameLib/Input/KeyInput.h"

// その他
#include "WindowManager.h"
#include <GameLib/Random.h>

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
	, m_pTestCanvas{ nullptr }
	, m_pTestUI{ nullptr }
	, m_player{ nullptr }
	, m_testNav{ 15.0f, 1.0f, 5.0f }
	, m_startIndex{ 0 }
	, m_goalIndex{ 0 }
{
}

GamePlayScene::~GamePlayScene()
{
}

// 初期化関数
void GamePlayScene::Initialize()
{
	m_dimentionManager.Initialize();

	// オブジェクトの追加
	// カメラ
	m_camera = Generate();

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	GetCamera().SetMainCamera(cameraComponent);

	// プレイヤーを生成
	{
		m_player = Generate();
		m_player->AddComponent<Player>();
		m_player->AddComponent<CapsuleCollider>()->SetLayer(10);
		auto rand = m_player->AddComponent<BoxCollider>();// 着地判定用のコライダーを生成
		rand->SetTrigger(true);
		rand->SetLocalPos({ 0, -1.05f, 0 });
		rand->SetLocalSize({ 1, 0.1f, 1 });
		m_player->AddComponent<ModelComponent>()->SetModel("Template_Capsule");
		m_player->AddComponent<RigidBody>();
		m_player->AddComponent<ChangeColliderComponent>();

		m_camera->AddComponent<TPSCamera>()->SetTarget(m_player->GetComponent<Transform>());
	}

//	auto* cube = Generate();
//	GameObjectSaver::LoadObject(cube, "Test.json");

	GenerateCube({ 0, -3, 0 }, 0, { 3, 1, 3 });
	GenerateCube({ 0, -3, -10 }, 0, { 3, 1, 3 });


	for (int i = 0; i < 10; i++)
	{
		auto cube = GenerateCube(
			{ Random::GetFloat(-10, 10), Random::GetFloat(-3, 5), Random::GetFloat(-10, 10) }
			,0
			,{ 3.0f, 1.0f, 3.0f } 
			,{ 0, Random::GetFloat(0.0f, PI_F / 8), 0/*Random::GetFloat(0.0f, PI_F / 8)*/}
		);
		if (i == 0) cube->AddComponent<MoveComponent>()->SetFunc(TestMove);
	}

	// 次元管理クラスにカメラを渡す
	m_dimentionManager.SetCamera(m_camera->GetComponent<ProjectionSmoothCamera>());

	auto goal = Generate({ 10, 5, 1 });
	goal->AddComponent<CapsuleCollider>()->SetTrigger(true);

//	GenerateCube({ 0, -10, 0 }, 0, { 20, 1, 20 });

	m_testNav.Initialize();
}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	m_dimentionManager.Update();

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

	if (MouseInput::GetMouseDown(MOUSE_LEFT) || MouseInput::GetMouseDown(MOUSE_RIGHT))
	{
		// クリックしたオブジェクトを取得
		Ray ray = GetCamera().GetMainCamera()->GetRayToScreenPoint(MouseInput::GetScaledMousePoint());

		RaycastHit hit;

		uint64_t layerMask = ~((0xFFFFFFFFFFFFFFFF) & (1 << 10));

		if (!GetPhysics().RayCast(ray, 100.0f, hit, layerMask)) return;

		// クリックしたオブジェクトが着地候補オブジェクトなら
		if (auto comp = hit.collider->GetOwn()->GetComponent<LandingCandidatePoints>())
		{
			// インデックスを取得
			int i = m_testNav.GetIndex(comp);

			// 存在すれば
			if (i != -1)
			{
				if (MouseInput::GetMouseDown(MOUSE_LEFT))
				{
					// スタートを変更
					m_startIndex = i;
				}
				else
				{
					// g
					m_goalIndex = i;
				}
			 }
		}
	}

	m_testNav.Update();
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
//	renderer.Draw().Line({ 0, 0, 0 }, { 100, 100, 100 }, 0xFFFFFF);

	Transform* player = m_player->GetComponent<Transform>();

	// プレイヤーの前方向をテスト描画する
	DirectX::SimpleMath::Vector3 playerPos = player->GetWorldPosition();
	DirectX::SimpleMath::Vector3 playerForward = playerPos + player->GetForward() * 2.0f;

	static const float angle = PI_F / 6;
	static const DirectX::SimpleMath::Quaternion left = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle({ 0, 1, 0 }, angle);
	static const DirectX::SimpleMath::Quaternion right = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle({ 0, 1, 0 }, -angle);

	DirectX::SimpleMath::Vector3 playerLeftArrow = DirectX::SimpleMath::Vector3::Transform(-player->GetForward(), left);
	DirectX::SimpleMath::Vector3 playerRightArrow = DirectX::SimpleMath::Vector3::Transform(-player->GetForward(), right);

	renderer.Draw().Line(playerPos, playerForward, 0x00FFFF);
	renderer.Draw().Line(playerForward + playerLeftArrow * 0.5f, playerForward, 0x00FFFF);
	renderer.Draw().Line(playerForward + playerRightArrow * 0.5f, playerForward, 0x00FFFF);

	m_startIndex = m_testNav.GetIndex(m_player->GetComponent<Player>()->GetLandingPoints());

	static std::vector<NavigationGraph::Edge> edges;
	edges = AStarPathFinder::MakePath(m_testNav, m_startIndex, m_goalIndex);

//	if (m_startIndex >= 0 && m_startIndex < m_testNav.GetGraph().size()) m_testNav.DebugDraw(m_testNav.GetGraph()[m_startIndex], renderer);
	m_testNav.DebugDraw(edges, renderer, 0xAA00AA);
}

// 終了関数
void GamePlayScene::Finalize()
{
	m_testNav.ResetGraph();
}

void GamePlayScene::RegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーに追加
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.AddChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	// 
	if (component->GetID() == ComponentID::LandingCandidatePoints)
	{
		m_testNav.AddNode(static_cast<LandingCandidatePoints*>(component));
	}
}

void GamePlayScene::UnRegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーから削除
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.RemoveChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
	if (component->GetID() == ComponentID::LandingCandidatePoints)
	{
		m_testNav.RemoveNode(static_cast<LandingCandidatePoints*>(component));
	}
}

void GamePlayScene::InitializeUITest()
{
	// UIの初期化
	m_pTestCanvas = GetUI().CreateCanvas();

	{
		auto* testUI = m_pTestCanvas->Generate<UIObject>();
		m_pTestUI = testUI;

		RectTransform* rect = testUI->GetComponent<RectTransform>();

		rect->SetAnchor({ 0.5, 0.5 });
		rect->SetAnchoredPosition({ 0, 0 });
		rect->SetPivot({ 0, 0 });
		rect->SetSize({ 360, 360 });

		auto* imageUI = testUI->AddComponent<ImageUI>();
		testUI->AddComponent<ButtonUI>()->SetOnClick(
			[this]
			{
				WindowManager::Instance().SwitchScreenMode(
					ResourceManager::Instance().GetResources()->GetWindow(),
					m_pGame
				);
			}
		);
		imageUI->SetTexture(ResourceManager::Instance().GetTexture("QR"));
		//imageUI->SetAlpha(0.5f);
		imageUI->SetColor(0x00FF00);

		auto* child = m_pTestCanvas->Generate<UIObject>();
		RectTransform* childRect = child->GetComponent<RectTransform>();
		childRect->SetParent(rect);

		childRect->SetAnchor({ 0.5f, 0.5f });
		childRect->SetAnchoredPosition({ 0, 0 });
		childRect->SetPivot({ 0.5f, 0.5f });
		childRect->SetSize({ 512, 256 });

		auto* childText = child->AddComponent<ImageUI>();
		child->AddComponent<ButtonUI>();
		childText->SetTexture(ResourceManager::Instance().GetTexture("TemplateImage"));
	//	childText->SetAlpha(0.5f);
		childText->SetColor(0xFF0000);
	}
}

GameObject* GamePlayScene::GenerateCube(DirectX::SimpleMath::Vector3 position, int type, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot)
{
	auto cube = Generate(position);

	switch (type)
	{
	case 0:
		cube->AddComponent<BoxCollider>();
		cube->AddComponent<ModelComponent>()->SetModel("Template_Cube");
		break;
	case 1:
		cube->AddComponent<SphereCollider>();
		cube->AddComponent<ModelComponent>()->SetModel("Template_Sphere");
		break;
	case 2:
		cube->AddComponent<CapsuleCollider>();
		cube->AddComponent<ModelComponent>()->SetModel("Template_Capsule");
		break;
	default:
		break;
	}

	cube->GetComponent<ModelComponent>()->SetAlpha(0.5f);

	cube->GetComponent<Transform>()->SetLocalScale(scale);
	cube->GetComponent<Transform>()->SetLocalEulerAngle({ rot });
	cube->AddComponent<ChangeColliderComponent>();

	// テスト用
	cube->AddComponent<LandingCandidatePoints>();

	cube->SetTag("Floor");

	return cube;
}

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
