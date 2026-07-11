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
EditScene::EditScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_gui{ this, [this]() { TestPlay(); } }
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
	// 初期位置
	SetMainScreenStartPoint({ 8, 26 });

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
	m_sceneViewCamera = game->AddComponent<SceneCamera>();
}

EditScene::~EditScene()
{
}

// 初期化関数
void EditScene::Initialize()
{
	// GUIのリセット
	m_gui.Reset();

	ObjectLoader::LoadSceneFromFile(L"Resources/Scenes/TestPlayScene.scene", this);
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
	m_gui.DrawViews(m_sceneView->GetShaderResourceView(), GetMainRenderTarget()->GetShaderResourceView());

	// シーンビューカメラ
	m_sceneViewCamera->SetMovable(m_gui.GetWindowType() == EditGUI::WindowType::SceneView);

	// カメラ
	Ray ray = m_sceneViewCamera->GetRayToScreenPoint(GetMousePointOnMainScreen());
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

void EditScene::TestPlay()
{
	// 保存
	ObjectSaver::SaveSceneToFile(L"Resources/Scenes/TestPlayScene.scene", this);

	// シーン変更
	ChangeScene("TestPlay");
}
