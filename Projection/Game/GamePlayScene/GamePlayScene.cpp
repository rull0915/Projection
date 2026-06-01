#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

#include "GameLib/GameObject/Components/Camera/Derived/StandardCamera.h"
#include "Camera/ProjectionSmoothCamera.h"
#include "Player/Player.h"

#include "GameLib/Random.h"

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
	, m_pTestCanvas{ nullptr }
	, m_pTestUI{ nullptr }
{
}

GamePlayScene::~GamePlayScene()
{
}

// 初期化関数
void GamePlayScene::Initialize()
{
	// オブジェクトの追加
	// カメラ
	m_camera = Generate();
	// TPSカメラに設定
//	auto cameraComponent = m_camera->AddComponent<TargetCamera>();
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	// メインカメラに設定
	GetCamera().SetMainCamera(cameraComponent);

	// オブジェクトを生成
	auto player = Generate();
//	player->AddComponent<ModelComponent>()->SetModel("Player");
	player->AddComponent<Player>();
	auto box = player->AddComponent<BoxCollider>();
	box->SetLocalSize({ 2, 2, 2 });
	box->SetLayer(1);
//	player->AddComponent<BoxCollider2D>()->SetLocalSize({ 2, 2 });
	player->AddComponent<RigidBody>()->SetUseGravity(false);
//	player->AddComponent<RigidBody2D>();

	// カメラをプレイヤーの子に
	m_camera->GetComponent<Transform>()->SetParent(player->GetComponent<Transform>());
//	m_camera->GetComponent<Transform>()->SetLocalPosition({ 0, 5, -8 });

	// ターゲットをプレイヤーに
//	cameraComponent->SetTarget(player);

	for (int i = 0; i < 50; i++) 
		GenerateCube(
			{ Random::GetFloat(-50, 50), Random::GetFloat(-50, 50), Random::GetFloat(-50, 50) },
			{ Random::GetFloat(1, 10), Random::GetFloat(1, 5), Random::GetFloat(1, 10) }
		);
}

// 更新関数 
void GamePlayScene::Update(float elapsedTime)
{
	RaycastHit hit;

	if (MouseInput::GetMouseDown(MOUSE_LEFT))
	{
		// マウスの位置へのRayを生成
		Ray ray = GetCamera().GetMainCamera()->GetRayToScreenPoint(MouseInput::GetMousePoint());

		// 当たり判定
		RaycastHit hit;
		
		if (GetPhysics().RayCast(ray, 1000.0f, hit, 0b01))
		{
			hit.collider;
			//auto obj = static_cast<GameObject*>(hit.collider->GetOwn());
			//obj->Destroy();

			// オブジェクトを生成
			GameObject* obj = Generate();

			// 位置を衝突点に
			obj->GetComponent<Transform>()->SetWorldPosition(hit.point);
			obj->GetComponent<Transform>()->SetLocalScale({ 0.3f, 0.3f, 0.3f });

			// コンポーネントの追加
			obj->AddComponent<ModelComponent>()->SetModel("Template_Sphere");
//			obj->AddComponent<SphereCollider>();
		}
	}
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
	renderer;
}

// 終了関数
void GamePlayScene::Finalize()
{
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
		testUI->AddComponent<ButtonUI>();
		imageUI->SetTexture(ResourceManager::Instance().GetTexture("TemplateImage"));
	//	imageUI->SetAlpha(0.5f);
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

void GamePlayScene::GenerateCube(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale)
{
	auto cube = Generate();
	cube->AddComponent<ModelComponent>()->SetModel("Template_Cube");
	cube->GetComponent<Transform>()->SetLocalPosition(position);
	cube->GetComponent<Transform>()->SetLocalScale(scale);
	cube->GetComponent<Transform>()->SetLocalEulerAngle({ PI_F / 4, PI_F / 4, 0 });
	cube->AddComponent<BoxCollider>();
//		->SetHeight(3.0f);
//	cube->AddComponent<CircleCollider2D>();
}
