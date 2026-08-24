//====================================================//
// ファイル名  : PlaySceneManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "PlaySceneManager.h"

#include "Player/Player.h"
#include "Camera/TPSCamera.h"
#include "Camera/CameraCorrection.h"

#include "Input/KeyInput.h"
#include "Scene/Scene.h"

#include "Effect/ClearEffect.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
PlaySceneManager::PlaySceneManager(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_cameraName{ "Camera" }
	, m_playerName{ "Player" }
	, m_camera{ nullptr }
	, m_player{ nullptr }
	, m_dimentionManager{ nullptr }
	, m_enemyManager{ nullptr }
	, m_eventIds{}
{
	ADD_PROPERTY(m_cameraName);
	ADD_PROPERTY(m_playerName);
}

void PlaySceneManager::Awake()
{
	// 開始イベントの追加
	m_eventIds.push_back(
	REngine::EventBus<GamePlayEvent>::Register(
		GamePlayEvent::Start,
		[this]() {
			// Playerをアクティブ化
			if (auto* p = m_player->GetComponent<Player>())
			{
				p->SetActive(true);
			}
		}
	));

	// ゴールイベントの追加
	m_eventIds.push_back(
		REngine::EventBus<GamePlayEvent>::Register(
			GamePlayEvent::Goal,
			[this]()
			{
				// クリアUIの出現
				if (auto* clear = GetComponent<ClearEffect>())
				{
					clear->SetActive(true);
				}
			}
		));
}

void PlaySceneManager::OnDestroy()
{
	// ゴールイベントの削除
	for (auto& id : m_eventIds)
	{
		REngine::EventBus<GamePlayEvent>::Remove(id);
	}
}

// 最初のUpdate関数の直線に一度呼ばれます
void PlaySceneManager::Start()
{
	// 各要素取得する
	REngine::Scene* scene = static_cast<REngine::GameObject*>(GetOwn())->GetScene();

	// プレイヤー
	m_player = scene->GetObjectFinder()->FindWithNameInWorld(m_playerName);

	// 非アクティブにしておく
	if (auto* p = m_player->GetComponent<Player>())
	{
		p->SetActive(false);
	}

	// カメラ
	m_camera = scene->GetObjectFinder()->FindWithNameInWorld(m_cameraName);

	// 敵管理
	m_enemyManager = static_cast<EnemyManager*>(scene->GetComponentRegister()->GetComponent<EnemyManager>());

	// 次元管理
	m_dimentionManager = static_cast<DimentionManager*>(scene->GetComponentRegister()->GetComponent<DimentionManager>());
}

// 毎フレーム呼ばれます
void PlaySceneManager::Update(const REngine::GameTimer& gameTimer)
{
	gameTimer;

	// Qキーで次元の変更
	if (REngine::Input::Key::GetDown(REngine::Input::Key::Code::Q))
	{
		TryChangeDimention();
	}

	// Rキーでリトライ
	if (REngine::Input::Key::GetDown(REngine::Input::Key::Code::R))
	{
	}
}

// 次元の切り替えを行う関数
void PlaySceneManager::TryChangeDimention()
{
	// 切り替え中なら何もしない
	if (m_dimentionManager->IsChanging()) return;

	// 2D->3D
	if (m_dimentionManager->GetIs2D())
	{
		// TPSをアクティブ化
		m_camera->GetComponent<TPSCamera>()->SetActive(true);
		m_camera->GetComponent<CameraCorrection>()->SetActive(true);

		// カメラの親子関係を解除
		m_camera->GetComponent<REngine::Transform>()->SetParent(nullptr);
	}
	else
	{
		// TPSカメラを非アクティブ化
		m_camera->GetComponent<TPSCamera>()->SetActive(false);
		m_camera->GetComponent<CameraCorrection>()->SetActive(false);

		// カメラをプレイヤーの子に設定
		m_camera->GetComponent<REngine::Transform>()->SetParent(m_player->GetComponent<REngine::Transform>());
	}

	// 次元の切り替え
	m_dimentionManager->ChangeDimention();

	// プレイヤーの切り替え
	m_player->GetComponent<Player>()->ChangeDimention();

	// 敵の切り替え
	m_enemyManager->ChangeDimantion();
}

