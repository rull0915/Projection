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

#include "Saver/ObjectSaver.h"
#include "Input/KeyInput.h"

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

	// 再生しない設定に
	SetPlayFlag(false);

	// メインスクリーンの場所を設定
	SetMainScreenStartPoint({});

	// サイズ
	SetMainScreenScale({ 0.45f, 0.45f });
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

	// 3つのウィンドウを描画
	m_gui.DrawWindows();

	// 2つのビューを描画
	m_gui.DrawViews(m_sceneView->GetShaderResourceView(), GetMainRenderTarget()->GetShaderResourceView());

	// Ctrl + S で保存
	if (
		Input::Key::Get(Input::State::Press, Input::Key::Code::LeftControl) &&
		Input::Key::Get(Input::State::Down, Input::Key::Code::S))
	{
		ObjectSaver::SaveSceneToFile(L"Resources/Scenes/TestScene.scene", this);
	}
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
