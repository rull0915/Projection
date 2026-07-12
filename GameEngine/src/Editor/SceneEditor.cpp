//====================================================//
// ファイル名  : SceneEditor.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要       : SceneEditorシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Editor/SceneEditor.h"

#include "System/ResourceManager.h"
#include "System/WindowManager.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneRenderer.h"

#include "Input/MouseInput.h"

// セーブロード
#include "Editor/Saver/ObjectSaver.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SceneEditor::SceneEditor(Scene* pScene)
	: m_pScene{ pScene }
	, m_isActive{ false }
	, m_isPlaying{ false }
	, m_gui{ m_pScene, [this]() { TestPlay(); } }
	, m_sceneView{ std::make_unique<RenderTarget>() }
	, m_sceneViewCamera{ nullptr }
{
	// ゲームビューの作成
	m_sceneView->Create(
		ResourceManager::Instance().GetResources()->GetD3DDevice(),
		WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight()
	);

	// ゲームビューカメラの作成
	auto camera = m_pScene->GetFactory()->Generate();

	// ヒエラルキーウィンドウ非表示
	camera->SetInvincible(true);

	// カメラの追加
	m_sceneViewCamera = camera->AddComponent<SceneCamera>();

	// 専用シーンを登録
	SceneManager::Instance().RegisterScene("Edit__", L"Resources/Scenes/TestPlayScene.scene");
}

SceneEditor::~SceneEditor()
{
}

// 初期化関数
void SceneEditor::Initialize()
{
	// GUIのリセット
	m_gui.Reset();

	// エディットシーンへ
	SceneManager::Instance().RequestSceneChange("Edit__");

	// エディットモードへ
	m_pScene->SetUpdateMode(UpdateMode::Edit);

	// アクティブ化
	m_isActive = true;

	// プレイフラグのリセット
	m_isPlaying = false;

	// メインスクリーンの描画を無効化
	m_pScene->GetMainScreen()->SetIsDraw(false);

	// マウスの表示を設定
	Input::Mouse::SetMouseVisible(true);
	Input::Mouse::SetMode(DirectX::Mouse::MODE_ABSOLUTE);
}

// 更新関数
void SceneEditor::Update(const GameTimer& gameTimer)
{
	if (!m_isActive) return;

	gameTimer;

	// 3つのウィンドウを描画
	m_gui.DrawWindows();

	// 2つのビューを描画
	m_gui.DrawViews(m_sceneView->GetShaderResourceView(), m_pScene->GetMainScreen()->GetRenderTarget()->GetShaderResourceView());

	// シーンビューカメラ
	m_sceneViewCamera->SetMovable(m_gui.GetWindowType() == EditGUI::WindowType::SceneView);
}

// 描画関数
void SceneEditor::Render(Renderer& renderer)
{
	if (!m_isActive) return;

	// ゲームビューへの描画
	m_pScene->GetSceneRenderer()->RenderWithContext(
		{
			m_sceneViewCamera, m_sceneView.get(), WindowManager::Instance().GetBackGroundColor(),
			DrawFlag::World | DrawFlag::UI | DrawFlag::ColliderDebug | DrawFlag::RectDebug
		}, renderer);
}

void SceneEditor::TestPlay()
{
	// プレイ中なら何もしない
	if (m_isPlaying) return;

	// フラグをオン
	m_isPlaying = true;

	// 保存
	ObjectSaver::SaveSceneToFile(L"Resources/Scenes/TestPlayScene.scene", m_pScene);

	// シーンを読み込みなおす
	SceneManager::Instance().RequestSceneChange("Edit__");

	// プレイモードへ
	m_pScene->SetUpdateMode(UpdateMode::Play);
}
