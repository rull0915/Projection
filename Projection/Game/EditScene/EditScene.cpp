//====================================================//
// ファイル名  : EditScene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : EditSceneシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EditScene.h"

#include "Game/Game.h"
#include "Components/World/Camera/Derived/StandardCamera.h"
#include "Components/World/Components.h"

#include "System/ResourceManager.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
EditScene::EditScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_gui{ this }
	, m_camera{ nullptr }
	, m_testObject{ nullptr }
	, m_sceneView{ std::make_unique<RenderTarget>() }
	, m_sceneViewCamera{ nullptr }
{
	// メインスクリーンを描画しない設定に
	SetDrawMainScreen(false);
}

EditScene::~EditScene()
{
}

// 初期化関数
void EditScene::Initialize()
{
	// ゲームビューの作成
	m_sceneView->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight()
	);

	// ゲームビューカメラの作成
	auto game = Generate();
	// ヒエラルキー非表示
	game->SetInvincible(true);
	// カメラの追加
	m_sceneViewCamera = game->AddComponent<StandardCamera>();

	// オブジェクトの追加
	// カメラ
	m_camera = Generate({ 0, 0, 10 });
	m_camera->SetName("Camera");

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<StandardCamera>();
	SetMainCamera(cameraComponent);

	auto test = Generate();
	test->SetName("Child");
	test->GetComponent<Transform>()->SetParent(m_camera->GetComponent<Transform>());

	m_testObject = Generate({ 0, 0, 0 });
	m_testObject->AddComponent<BoxCollider>();
	m_testObject->AddComponent<ModelComponent>()->SetModel("Cube");
}

// 更新関数
void EditScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	// ヒエラルキーの描画を開始
	m_gui.StartHierarchy();

	// World空間のオブジェクト
	m_gui.DrawObjects(GetObjectManager());

	// UI空間のオブジェクト
	m_gui.DrawObjects(GetUIManager());

	m_gui.EndWindow();

	// インスペクターの描画を開始
	m_gui.StartInspector();

	m_gui.DrawGameObjectOnInspector(m_gui.GetSelected());

	m_gui.EndWindow();

	// シーンビューの描画を開始
	m_gui.StartSceneView();

	m_gui.DrawImage(m_sceneView->GetShaderResourceView(), {576, 324});

	m_gui.EndWindow();

	// ゲームビューの描画を開始
	m_gui.StartGameView();

	m_gui.DrawImage(GetMainRenderTarget()->GetShaderResourceView(), { 576, 324 });

	m_gui.EndWindow();
}

// 描画関数
void EditScene::Render(Renderer& renderer)
{
	// ゲームビューへの描画
	RenderWithContext(
		{
			m_sceneViewCamera, m_sceneView.get(), WindowManager::Instance().GetBackGroundColor(),
			DrawFlag::World | DrawFlag::UI | DrawFlag::ColliderDebug | DrawFlag::RectDebug
		}, renderer);
}

// 終了関数
void EditScene::Finalize()
{
}
