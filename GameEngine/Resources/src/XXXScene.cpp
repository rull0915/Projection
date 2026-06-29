//====================================================//
// ファイル名  : $safeitemname$Scene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : $shortdate$
//
// 概要       : $safeitemname$シーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "$safeitemname$.h"

#include "Game/Game.h"
#include "Components/World/Camera/Derived/StandardCamera.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
$safeitemname$Scene::$safeitemname$Scene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
{
}

$safeitemname$Scene::~$safeitemname$Scene()
{
}

// 初期化関数
void $safeitemname$Scene::Initialize()
{
	// オブジェクトの追加
	// カメラ
	m_camera = Generate({ 0, 10, 0});

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<StandardCamera>();
	SetMainCamera(cameraComponent);
}

// 更新関数
void $safeitemname$Scene::Update(const GameTimer& gameTimer)
{
	gameTimer;
}

// 描画関数
void $safeitemname$Scene::Render(Renderer& renderer)
{
}

// 終了関数
void $safeitemname$Scene::Finalize()
{
}
