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

#include "Input/InputSystem.h"
#include "Scene/Scene.h"

#include "Effect/ClearEffect.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
PlaySceneManager::PlaySceneManager(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_camera{}
	, m_player{}
	, m_dimentionManager{}
	, m_eventIds{}
{
	ADD_PROPERTY(PlaySceneManager, m_camera);
	ADD_PROPERTY(PlaySceneManager, m_player);
	ADD_PROPERTY(PlaySceneManager, m_dimentionManager);
}

void PlaySceneManager::Awake()
{
	// 開始イベントの追加
	m_eventIds.push_back(
	RHEngine::EventBus<GamePlayEvent>::Register(
		GamePlayEvent::Start,
		[this]() {
			if (!m_player) return;

			// Playerをアクティブ化
			if (auto* p = m_player->GetComponent<Player>())
			{
				p->SetActive(true);
			}
		}
	));

	// ゴールイベントの追加
	m_eventIds.push_back(
		RHEngine::EventBus<GamePlayEvent>::Register(
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
		RHEngine::EventBus<GamePlayEvent>::Remove(id);
	}
}

// 最初のUpdate関数の直線に一度呼ばれます
void PlaySceneManager::Start()
{
	// プレイヤーを非アクティブにしておく
	if (m_player)
	{
		if (auto p = m_player->GetComponent<Player>()) p->SetActive(false);
	}
}

// 毎フレーム呼ばれます
void PlaySceneManager::Update(const RHEngine::GameTimer& gameTimer)
{
	gameTimer;

	// Qキーで次元の変更
	if (RHEngine::Input::Custom::GetButtonDown("ChangeDimention"))
	{
		TryChangeDimention();
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
		m_camera->GetComponent<RHEngine::Transform>()->SetParent(nullptr);
	}
	else
	{
		// TPSカメラを非アクティブ化
		m_camera->GetComponent<TPSCamera>()->SetActive(false);
		m_camera->GetComponent<CameraCorrection>()->SetActive(false);

		// カメラをプレイヤーの子に設定
		m_camera->GetComponent<RHEngine::Transform>()->SetParent(m_player->GetComponent<RHEngine::Transform>());
	}

	// 次元の切り替え
	m_dimentionManager->ChangeDimention();

	// プレイヤーの切り替え
	m_player->GetComponent<Player>()->ChangeDimention();
}

