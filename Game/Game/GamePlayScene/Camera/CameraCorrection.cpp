//====================================================//
// ファイル名  : CameraCorrection.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/13
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CameraCorrection.h"

#include "Scene/Scene.h"
#include "Physics/Ray.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
CameraCorrection::CameraCorrection(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_camera{ nullptr }
	, m_player{ nullptr }
	, m_pScene{ nullptr }
	, m_playerName{}
	, m_playerLayer{ 0 }
{
	ADD_PROPERTY(m_playerName);
	ADD_PROPERTY(m_playerLayer);
}

// 生成直後に一度呼ばれます
void CameraCorrection::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void CameraCorrection::Start()
{
	// TPSカメラを取得
	m_camera = GetComponent<TPSCamera>();

	// プレイヤーを取得
	m_pScene = static_cast<GameObject*>(GetOwn())->GetScene();

	// 探索
	m_player = m_pScene->GetObjectFinder()->FindWithNameInWorld(m_playerName);
}

// 毎フレーム呼ばれます
void CameraCorrection::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void CameraCorrection::LateUpdate(const GameTimer & gameTimer)
{
	// プレイヤーがなければ何もしない
	if (!m_player || !m_camera) return;

	// プレイヤーからカメラへRayを飛ばす

	// カメラの理想位置
	DirectX::SimpleMath::Vector3 cameraPos = m_camera->GetIdealPosition();

	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 playerPos = m_player->GetComponent<Transform>()->GetWorldPosition();

	// Rayの生成
	DirectX::SimpleMath::Vector3 direction = cameraPos - playerPos;
	direction.Normalize();

	Ray ray{ playerPos, direction };

	// 目標位置
	DirectX::SimpleMath::Vector3 result = cameraPos;

	// 衝突判定
	RaycastHit info;
	if (m_pScene->RayCast(ray, 5, &info, ~(1 << m_playerLayer)))
	{
		// カメラの位置を補正する
		result = info.point;
	}
	
	Transform* own = GetComponent<Transform>();
	Transform* target = m_player->GetComponent<Transform>();

	// 位置を設定する
	own->SetWorldPosition(result);

	// 回転を設定する
	DirectX::SimpleMath::Vector2 angle = m_camera->GetRotation();

	// 角度を設定
	own->SetLocalEulerAngle({ -angle.x, angle.y, 0.0f });

	// ターゲットの角度を設置
	target->SetLocalEulerAngle({ 0, angle.y, 0.0f });
}
