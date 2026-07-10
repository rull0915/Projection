//====================================================//
// ファイル名  : TitleScene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要       : Titleシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TitleScene.h"

#include "Game/Game.h"
#include "Components/World/Camera/Derived/StandardCamera.h"
#include "Components/UI/Graphics/TextUI.h"
#include "Components/UI/Graphics/ImageUI.h"
#include "Components/UI/Behaviour/ButtonUI.h"

#include "Managers/UI/Canvas.h"

#include "Input/MouseInput.h"
#include "Input/KeyInput.h"

#include "GameLib/Transition/SlideTransition.h"
#include "Components/World/Renderer/Skybox/SkyboxComponent.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
TitleScene::TitleScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
	, m_canvas{ nullptr }
{
}

TitleScene::~TitleScene()
{
}

// 初期化関数
void TitleScene::Initialize()
{
	// オブジェクトの追加
	// カメラ
	m_camera = Generate({ 0, 10, 0});

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<StandardCamera>();
	SetMainCamera(cameraComponent);

	m_camera->AddComponent<SkyboxComponent>()->SetTexture("Skybox");

	// キャンバスの生成
	m_canvas = GenerateCanvas();

	// UIの追加
	{
		// UIオブジェクトの生成
		auto ui = m_canvas->Generate();

		// 画像コンポーネントの追加
		auto i = ui->AddComponent<ImageUI>();
		i->SetTexture("Default");	// 描画する画像の設定	
		i->SetColor({ 0.45, 0.45, 0.45 });												// 色の設定	

		// テキストコンポーネントの追加	
		auto t = ui->AddComponent<TextUI>();
		t->SetFont("Default");	// 使うフォントの設定	
		t->SetFontSize(24);													// フォントサイズの設定	
		t->SetText("ToPlayScene");											// 文字列の設定	
		t->SetOrigin({ 0.5f, 0.5f });									// 原点を設定

		// Rectの設定変更	
		auto r = ui->GetComponent<RectTransform>();
		r->SetAnchor({ 0.5f, 0.6f });		// 基準の位置を設定	
		r->SetPivot({ 0.5f, 0.5f });		// 基準点に置かれるポイントを設定
		r->SetSize({ 400, 75 });			// サイズを設定

		// ボタンの追加	
		auto b = ui->AddComponent<ButtonUI>();
	}
	{
		// UIオブジェクトの生成
		auto ui = m_canvas->Generate();

		// 画像コンポーネントの追加
		auto i = ui->AddComponent<ImageUI>();
		i->SetTexture("Default");	// 描画する画像の設定	
		i->SetColor({ 0.8, 0.8, 0.8 });												// 色の設定	

		// テキストコンポーネントの追加	
		auto t = ui->AddComponent<TextUI>();
		t->SetFont("Default");	// 使うフォントの設定	
		t->SetFontSize(64);													// フォントサイズの設定	
		t->SetText("Projection");											// 文字列の設定	
		t->SetOrigin({ 0.5f, 0.5f });
		t->SetColor({ 0, 0, 0 });
		
		// Rectの設定変更	
		auto r = ui->GetComponent<RectTransform>();
		r->SetAnchor({ 0.5f, 0.4f });		// 基準の位置を設定	
		r->SetPivot({ 0.5f, 0.5f });		// 基準点に置かれるポイントを設定
		r->SetSize({ 600, 180 });			// サイズを設定// 原点を設定
//		r->SetRotation(DirectX::XMConvertToRadians(30));	// 
	}
}

// 更新関数
void TitleScene::Update(const GameTimer& gameTimer)
{
	gameTimer;

	// マウスを絶対モードに
	if (Input::Mouse::GetMode() == DirectX::Mouse::Mode::MODE_RELATIVE)
	{
		Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);

	}

	// Ctrl + Eキーでエディット
	if (Input::Key::Get(Input::State::Press, Input::Key::Code::LeftControl) &&
		Input::Key::Get(Input::State::Down, Input::Key::Code::E))
	{
		ChangeScene("Edit",
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(30)),
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(210))
		);
	}

	Input::Mouse::SetMouseVisible(true);
}

// 描画関数
void TitleScene::Render(Renderer& renderer)
{
}

// 終了関数
void TitleScene::Finalize()
{
}
