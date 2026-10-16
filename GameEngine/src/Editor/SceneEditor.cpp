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

#ifdef ENGINE_GUI

#include "Editor/SceneEditor.h"

#include "System/Render/GraphicsManager.h"
#include "System/WindowManager.h"
#include "Scene/SceneManager.h"

#include "Input/MouseInput.h"
#include "Editor/Saver/ObjectSaver.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	SceneEditor::SceneEditor(Scene* pScene, AssetManager& am)
		: m_pScene{ pScene }
		, m_isActive{ false }
		, m_isPlaying{ false }
		, m_gui{ m_pScene, am, [this]() { TestPlay(); } }
		, m_sceneView{ std::make_unique<RenderTarget>() }
		, m_sceneViewCamera{}
		, m_assetManager{ am }
	{
		// ゲームビューの作成
		m_sceneView->Create(
			GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice(),
			WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight()
		);

		// 専用シーンを登録
		SceneManager::Instance().RegisterScene("Edit__", L"Resources/Scenes/TestPlayScene.scene");

		// カメラの初期化
		m_sceneViewCamera.Start();

		// Projection行列の更新
		m_sceneViewCamera.UpdateView();
		m_sceneViewCamera.UpdateProj();
	}

	SceneEditor::~SceneEditor()
	{}

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

		// マウスの表示を設定
		Input::Mouse::SetMouseVisible(true);
		Input::Mouse::SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}

	void SceneEditor::Finalize()
	{
		// GUIのリセット
		m_gui.Reset();

		// プレイモードへ
		m_pScene->SetUpdateMode(UpdateMode::Play);

		// 非アクティブ化
		m_isActive = false;

		// プレイフラグのリセット
		m_isPlaying = false;

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
		m_gui.DrawViews(m_sceneView->GetShaderResourceView(), m_pScene->GetGameView(), &m_sceneViewCamera);

		// シーンビューカメラ
		m_sceneViewCamera.SetMovable(m_gui.GetWindowType() == EditGUI::WindowType::SceneView);

		// カメラの更新
		m_sceneViewCamera.Update(gameTimer);

		// ビュー行列の更新
		m_sceneViewCamera.UpdateView();

		static bool showShadowMap = false;

		// ToDelete
		if (Input::Mouse::Get(Input::Mouse::Button::Right) && Input::Mouse::GetDown(Input::Mouse::Button::Left))
		{
			showShadowMap = !showShadowMap;
		}
		if (showShadowMap) m_gui.DrawImageDebugWindow("ShadowMap", m_pScene->GetShadowMapTexture());
	}

	void SceneEditor::Render(Renderer& renderer)
	{
		// シーンビューを描画

		// 描画用コンテキストを用意
		RenderContext ctx;

		ctx.backColor = WindowManager::Instance().GetBackGroundColor();
		ctx.target = m_sceneView.get();
		ctx.flags = DrawFlag::World | DrawFlag::WorldDebug;
		ctx.clearTarget = true;
		ctx.cameraData = m_sceneViewCamera.GetData();

		// 描画
		m_pScene->DrawOnceCamera(ctx, renderer, true);
	}

	void SceneEditor::TestPlay()
	{
		// プレイ中なら何もしない
		if (m_isPlaying) return;

		// フラグをオン
		m_isPlaying = true;

		// GUIリセット
		m_gui.Reset();

		// 保存
		ObjectSaver saver(m_assetManager);

		saver.SaveSceneToFile(L"Resources/Scenes/TestPlayScene.scene", m_pScene);

		// シーンを読み込みなおす
		SceneManager::Instance().RequestSceneChange("Edit__");

		// プレイモードへ
		m_pScene->SetUpdateMode(UpdateMode::Play);
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
