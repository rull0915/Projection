#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

#include "GameLib/GameObject/Components/Camera/Derived/StandardCamera.h"
#include "Camera/ProjectionSmoothCamera.h"
#include "Player/Player.h"

#include "ChangeDimention/ChangeColliderComponent.h"

#include "GameLib/Input/KeyInput.h"
#include "GameLib/Random.h"

#include "GameLib/GameObject/Settings/WorldSetting2D.h"

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

	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	// メインカメラに設定
	GetCamera().SetMainCamera(cameraComponent);

	// オブジェクトを生成
	auto player = Generate();
	player->AddComponent<Player>();
	player->AddComponent<SphereCollider>();// ->SetLocalSize({ 2, 2, 2 });
	player->AddComponent<ModelComponent>()->SetModel("Template_Cube");

	player->AddComponent<RigidBody>()->SetUseGravity(true);
	player->AddComponent<ChangeColliderComponent>();
	m_stages.push_back(player);

	// カメラをプレイヤーの子に
	m_camera->GetComponent<Transform>()->SetParent(player->GetComponent<Transform>());
	m_camera->GetComponent<Transform>()->SetLocalPosition({ 0, 0, 10 });

	for (int i = 0; i < 10; i++) 
		GenerateCube(
			{ Random::GetFloat(-30, 30), Random::GetFloat(-10, 10), Random::GetFloat(-30, 30) },
			{ Random::GetFloat(1, 10), Random::GetFloat(1, 5), Random::GetFloat(1, 10) }
		);

	GenerateCube({ 0, -3, 0 }, { 10, 1, 10 });
}

// 更新関数 
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	RaycastHit hit;

	if (KeyInput::GetKeyDown(KeyCode::Q))
	{
		ProjectionSmoothCamera* camera = GetCamera().GetMainCamera()->GetComponent<ProjectionSmoothCamera>();

		// カメラがなければ何もしない
		if (!camera) return;
		if (camera->IsChanging()) return;

		// 2次元フラグ
		bool is3D = camera->GetProjectionType() == ProjectionType::Perspective;

		camera->ChangeProjectionMode();

		// 2次元世界の軸を変更
		if (is3D)
		{
			BaseCamera* mainCamera = GetCamera().GetMainCamera();

			auto& world2D = WorldSetting2D::Instance();
			DirectX::SimpleMath::Vector3 camRight = mainCamera->GetInverseView().Right();
			DirectX::SimpleMath::Vector3 camUp = mainCamera->GetInverseView().Up();
			world2D.SetAxis(camRight, camUp);

			// コライダーを変更
			for (auto& object : m_stages)
			{
				object->GetComponent<ChangeColliderComponent>()->Change3DTo2D(mainCamera);
			}
		}
		else
		{
			// コライダーを変更
			for (auto& object : m_stages)
			{
				object->GetComponent<ChangeColliderComponent>()->Change2DTo3D();
			}
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

void GamePlayScene::GenerateCube(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot)
{
	auto cube = Generate();
	cube->AddComponent<ModelComponent>()->SetModel("Template_Cube");
	cube->GetComponent<Transform>()->SetLocalPosition(position);
	cube->GetComponent<Transform>()->SetLocalScale(scale);
	cube->GetComponent<Transform>()->SetLocalEulerAngle({ rot });
	//cube->AddComponent<CapsuleCollider>()->SetHeight(3.0f);
	cube->AddComponent<BoxCollider>();
	//	->SetHeight(2.0f);
	cube->AddComponent<ChangeColliderComponent>();
//	auto collider = cube->AddComponent<CapsuleCollider2D>();

	m_stages.push_back(cube);
}
