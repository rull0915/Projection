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

#include "System/ResourceManager.h"
#include "System/WindowManager.h"

#include "Editor/Saver/ObjectSaver.h"
#include "Editor/Loader/ObjectLoader.h"

#include "Input/MouseInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
EditScene::EditScene(Scene* pScene)
	: m_pScene{ pScene }
	, m_gui{ m_pScene, [this]() { TestPlay(); } }
	, m_sceneView{ std::make_unique<RenderTarget>() }
	, m_sceneViewCamera{ nullptr }
{
	// メインスクリーンを描画しない設定に
	m_pScene->SetDrawMainScreen(false);

	// 再生しない設定に
	m_pScene->SetPlayFlag(false);

	// メインスクリーンの場所を設定
	m_pScene->SetMainScreenStartPoint({});

	// サイズ
	m_pScene->SetMainScreenScale({ 0.45f, 0.45f });
	// 初期位置
	m_pScene->SetMainScreenStartPoint({ 8, 26 });

	// ゲームビューの作成
	m_sceneView->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight()
	);

	// ゲームビューカメラの作成
	auto camera = m_pScene->Generate();

	// ヒエラルキーウィンドウ非表示
	camera->SetInvincible(true);

	// カメラの追加
	m_sceneViewCamera = camera->AddComponent<SceneCamera>();
}

EditScene::~EditScene()
{
}

// 初期化関数
void EditScene::Initialize()
{
	// GUIのリセット
	m_gui.Reset();

	ObjectLoader::LoadSceneFromFile(L"Resources/Scenes/TestPlayScene.scene", m_pScene);
}

// 更新関数
void EditScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	// マウスを絶対モードに固定する
	Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);

	// 3つのウィンドウを描画
	m_gui.DrawWindows();

	// 2つのビューを描画
	m_gui.DrawViews(m_sceneView->GetShaderResourceView(), m_pScene->GetMainRenderTarget()->GetShaderResourceView());

	// シーンビューカメラ
	m_sceneViewCamera->SetMovable(m_gui.GetWindowType() == EditGUI::WindowType::SceneView);
}

// 描画関数
void EditScene::Render(Renderer& renderer)
{
	// ゲームビューへの描画
	m_pScene->RenderWithContext(
		{
			m_sceneViewCamera, m_sceneView.get(), WindowManager::Instance().GetBackGroundColor(),
			DrawFlag::World | DrawFlag::UI | DrawFlag::ColliderDebug | DrawFlag::RectDebug
		}, renderer);
}

void EditScene::TestPlay()
{
	// 保存
	ObjectSaver::SaveSceneToFile(L"Resources/Scenes/TestPlayScene.scene", m_pScene);
}
