#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

#include "Camera/ProjectionSmoothCamera.h"
#include "Camera/TPSCamera.h"
#include "Player/Player.h"

#include "ChangeDimention/ChangeColliderComponent.h"

#include "WindowManager.h"
#include "GameLib/Input/KeyInput.h"

#include <GameLib/Random.h>

#include <string>
#include <format>

#include "GameLib/GameObject/Others/GameObjectSaver.h"

// コンストラクタ
GamePlayScene::GamePlayScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
	, m_pTestCanvas{ nullptr }
	, m_pTestUI{ nullptr }
	, m_player{ nullptr }
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

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<ProjectionSmoothCamera>();
	GetCamera().SetMainCamera(cameraComponent);

	// プレイヤーを生成
	{
		m_player = Generate();
		m_player->AddComponent<Player>();
		m_player->AddComponent<CapsuleCollider>();
		auto rand = m_player->AddComponent<BoxCollider>();// 着地判定用のコライダーを生成
		rand->SetTrigger(true);
		rand->SetLocalPos({ 0, -1.05f, 0 });
		rand->SetLocalSize({ 1, 0.1f, 1 });
		m_player->AddComponent<ModelComponent>()->SetModel("Template_Capsule");
		m_player->AddComponent<RigidBody>();
		m_player->AddComponent<ChangeColliderComponent>();

		m_camera->AddComponent<TPSCamera>()->SetTarget(m_player->GetComponent<Transform>());
	}

	auto* cube = Generate();
//	GameObjectSaver::LoadObject(cube, "Test.json");

	GenerateCube({ 0, -3, 0 }, 0, { 3, 1, 3 });
	GenerateCube({ 0, -3, -10 }, 0, { 3, 1, 3 });


	for (int i = 0; i < 0; i++)
	{
		auto cube = GenerateCube({ Random::GetFloat(-10, 10), Random::GetFloat(0, 5), Random::GetFloat(-10, 10) }, Random::Get(0, 2), { 1.0f, 1.0f, 1.0f }, { Random::GetFloat(0.0f, PI_F), Random::GetFloat(0.0f, PI_F), Random::GetFloat(0.0f, PI_F) });
		cube->AddComponent<RigidBody>();
	}

	// 次元管理クラスにカメラを渡す
	m_dimentionManager.SetCamera(m_camera->GetComponent<ProjectionSmoothCamera>());

	auto goal = Generate({ 10, 5, 1 });
	goal->AddComponent<CapsuleCollider>()->SetTrigger(true);

//	GenerateCube({ 0, -10, 0 }, 0, { 20, 1, 20 });

//	InitializeUITest();
}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	m_dimentionManager.Update();

	if (KeyInput::GetKeyDown(KeyCode::Q))
	{
		TryChangeDimention();
	}

	// マウス左クリック時
	if (MouseInput::GetMouseDown(MOUSE_LEFT))
	{
		// マウスの位置へrayを飛ばす
		Ray ray = GetCamera().GetMainCamera()->GetRayToScreenPoint(MouseInput::GetScaledMousePoint());
		RaycastHit hit;

		// 何かに衝突していれば
		if (GetPhysics().RayCast(ray, 100.0f, hit))
		{
			//// 衝突点にオブジェクトを作る
			//auto obj = Generate(hit.point);

			//obj->AddComponent<ModelComponent>()->SetModel("Template_Sphere");
			//obj->GetComponent<Transform>()->SetLocalScale({ 0.2f, 0.2f, 0.2f });

			// テスト保存
			GameObjectSaver::SaveObject(static_cast<GameObject*>(hit.collider->GetOwn()), "Test.json");
		}
	}
}

// 描画関数
void GamePlayScene::Render(Renderer& renderer)
{
	Transform* player = m_player->GetComponent<Transform>();

	// プレイヤーの前方向をテスト描画する
	DirectX::SimpleMath::Vector3 playerPos = player->GetWorldPosition();
	DirectX::SimpleMath::Vector3 playerForward = playerPos + player->GetForward() * 2.0f;

	static const float angle = PI_F / 6;
	static const DirectX::SimpleMath::Quaternion left = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle({ 0, 1, 0 }, angle);
	static const DirectX::SimpleMath::Quaternion right = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle({ 0, 1, 0 }, -angle);

	DirectX::SimpleMath::Vector3 playerLeftArrow = DirectX::SimpleMath::Vector3::Transform(-player->GetForward(), left);
	DirectX::SimpleMath::Vector3 playerRightArrow = DirectX::SimpleMath::Vector3::Transform(-player->GetForward(), right);

	renderer.Draw().Line( playerPos, playerForward, 0x00FFFF );
	renderer.Draw().Line( playerForward + playerLeftArrow * 0.5f, playerForward, 0x00FFFF );
	renderer.Draw().Line( playerForward + playerRightArrow * 0.5f, playerForward, 0x00FFFF );

	renderer;

	auto tex = ResourceManager::Instance().GetTexture("TemplateImage");

	renderer.SetAlpha(1.0f);

	DirectX::SimpleMath::Vector2 mouse = MouseInput::GetScaledMousePoint();

	std::wstring message = std::format(L"Mouse X: {} Y: {}", (int)mouse.x, (int)mouse.y);


	renderer.Draw().Text()
		.Extend({ 0.3f, 0.3f })
		.Execute(ResourceManager::Instance().GetSpriteFont("Default"), message.c_str(), {0, 0}, 0xFFFFFF);

	RECT rc;
	GetClientRect(ResourceManager::Instance().GetResources()->GetWindow(), &rc);
	
	int width  = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	message = std::format(L"Window W: {} H: {}", width, height);

	renderer.Draw().Text()
		.Extend(0.2f)
		.Execute(ResourceManager::Instance().GetSpriteFont("Default"), message.c_str(), { Screen::CENTER_X, 0 });
}

// 終了関数
void GamePlayScene::Finalize()
{
}

void GamePlayScene::RegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーに追加
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.AddChangeComponent(static_cast<ChangeColliderComponent*>(component));
	}
}

void GamePlayScene::UnRegisterComponentOnDerived(BaseComponent* component)
{
	// 次元変更マネージャーから削除
	if (component->GetID() == ComponentID::ChangeColliderComponent)
	{
		m_dimentionManager.RemoveChangeComponent(static_cast<ChangeColliderComponent*>(component));
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
