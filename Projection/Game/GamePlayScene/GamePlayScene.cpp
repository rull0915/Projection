#include "pch.h"

#include "GamePlayScene.h"
#include "../Game.h"

#include "Camera/ProjectionSmoothCamera.h"
#include "Camera/TPSCamera.h"
#include "Player/Player.h"

#include "ChangeDimention/ChangeColliderComponent.h"

#include "GameLib/Input/KeyInput.h"

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
		m_player->AddComponent<BoxCollider>();
		auto rand = m_player->AddComponent<BoxCollider>();// 着地判定用のコライダーを生成
		rand->SetTrigger(true);
		rand->SetLocalPos({ 0, -0.55f, 0 });
		rand->SetLocalSize({ 1, 0.1f, 1 });
		m_player->AddComponent<ModelComponent>()->SetModel("Template_Cube");
		m_player->AddComponent<RigidBody>();
		m_player->AddComponent<ChangeColliderComponent>();

		m_camera->AddComponent<TPSCamera>()->SetTarget(m_player->GetComponent<Transform>());
	}

	GenerateCube({ 0, -3, 0 }, { 3, 1, 3 });
	GenerateCube({ 0, -3, -10 }, { 3, 1, 3 });


	for (int i = 0; i < 1000; i++) GenerateCube({ i + 0.0f, 0, 0 }, { 1.0f, 1.0f, 1.0f });

	// 
	m_dimentionManager.SetCamera(m_camera->GetComponent<ProjectionSmoothCamera>());
}

// 更新関数
void GamePlayScene::Update(const GameTimer& gameTimer)
{
	m_dimentionManager.Update();

	if (KeyInput::GetKeyDown(KeyCode::Q))
	{
		TryChangeDimention();
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

	if (m_player->GetComponent<Player>()->CanJump())
	{
		renderer.Draw().Text()
			.Rect({ 0, 0 }, { 150, 40 })
			.Execute(ResourceManager::Instance().GetSpriteFont("Default"), L"CanJump", 0xFFFFFF);
	}

	renderer;

	//renderer.Draw().Sprite()
	//	//.Rotate(PI_F / 2)
	//	.Origin(Origin::Type::Center)
	//	.Extend({ 1.0f, 2.0f })
	//	.Execute(
	//	ResourceManager::Instance().GetTexture("TemplateImage"), { 100, 100 }, 0xFFFFFF
	//);
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
	auto cube = Generate(position);
	cube->AddComponent<ModelComponent>()->SetModel("Template_Cube");
	cube->GetComponent<Transform>()->SetLocalScale(scale);
	cube->GetComponent<Transform>()->SetLocalEulerAngle({ rot });
	cube->AddComponent<BoxCollider>();
	cube->AddComponent<ChangeColliderComponent>();

	cube->SetTag(L"Floor");
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
