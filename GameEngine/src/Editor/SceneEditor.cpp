/**
 * @file    SceneEditor.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/05
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "Editor/SceneEditor.h"

#include "System/Render/GraphicsManager.h"
#include "System/WindowManager.h"
#include "Scene/SceneManager.h"

#include "Input/MouseInput.h"
#include "Input/KeyInput.h"
#include "Editor/Saver/ObjectSaver.h"

#include "Assets/EngineDefaultAssets.h"
#include "Assets/Types/Texture.h"

namespace RHEngine
{
	// コンストラクタ
	SceneEditor::SceneEditor(Scene* pScene, AssetManager& assetManager)
		: m_pScene{ pScene }
		, m_isActive{ false }
		, m_isPlaying{ false }
		, m_gui{ m_pScene, assetManager, [this]() { TestPlay(); }, [this]() { PlayStop(); } }
		, m_sceneView{ std::make_unique<RenderTarget>() }
		, m_sceneViewCamera{}
		, m_assetManager{ assetManager }
		, m_oldMouseMode{}
	{
		// ゲームビューの作成
		m_sceneView->Create(
			GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice(),
			WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight()
		);

		// 専用シーンを登録
		SceneManager::Instance().RegisterScene("Edit__", L"Resources/Scenes/TestPlayScene.scene");

		// カメラの初期化
		m_sceneViewCamera.Initialize();

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

		// シーン描画のオフ
		SceneManager::Instance().SetDrawScene(false);
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

		// シーン描画のオン
		SceneManager::Instance().SetDrawScene(true);
	}

	// 更新関数
	void SceneEditor::Update(const GameTimer& gameTimer)
	{
		if (!m_isActive) return;

		gameTimer;

		// ロゴアセットを取得
		auto logo = m_assetManager.GetAssetFromUUID<Texture>(DefaultAsset::ENGINE_LOGO_SPRITE);

		// ツールバーの表示
		m_gui.DrawToolBar(logo->Get(), m_isPlaying);

		// ドッキング領域の描画
		m_gui.DrawDockingArea();

		// 3つのウィンドウを描画
		m_gui.DrawWindows();

		// パフォーマンスウィンドウを描画
		m_gui.DrawPerformanceWindow();

		// シーンビューを描画
		bool canMove = m_gui.DrawSceneView(m_sceneView->GetShaderResourceView(), &m_sceneViewCamera);
		
		// ゲームビューを描画
		bool clickedGameView = m_gui.DrawGameView(m_pScene->GetGameView());

		// シーンビューカメラ
		m_sceneViewCamera.SetMovable(canMove);

		// カメラの更新
		m_sceneViewCamera.Update();

		// ビュー行列の更新
		m_sceneViewCamera.UpdateView();

		// 実行中なら
		if (m_isPlaying)
		{
			// Escapeが入力されたらマウスを絶対モードに
			if (Input::Key::GetDown(Input::Key::Code::Escape))
			{
				// 今の状態を保持
				m_oldMouseMode = Input::Mouse::GetMode();

				// 変更
				Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
			}

			// ゲームビューがクリックされたら元に戻す
			if (clickedGameView)
			{
				Input::Mouse::SetMode(m_oldMouseMode);
			}
		}
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

	void SceneEditor::PlayStop()
	{
		// エディタ－を初期化
		Initialize();
	}
}	// namespace RHEngine

#endif // ENGINE_GUI
