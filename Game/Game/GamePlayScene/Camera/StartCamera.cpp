//====================================================//
// ファイル名  : StartCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/09
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "StartCamera.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include "TPSCamera.h"
#include "../GamePlayEvent.h"
#include "Common/EventBus.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
StartCamera::StartCamera(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_pTransform{ nullptr }
	, m_targetName{ "Player" }
	, m_target{ 0, 0, 0 }
	, m_radius{ 5.0f }
	, m_finalAngle{ DirectX::XM_PI / 2 }
	, m_angleVelocity{ DirectX::XM_PI }
	, m_initialHeight{ 5.0f }
	, m_fallVelocity{ 1.0f }
	, m_nowAngle{ 0.0f }
	, m_offset{ 0, 0, 0 }
{
	ADD_PROPERTY(m_targetName);
	ADD_PROPERTY(m_radius);
	ADD_PROPERTY(m_finalAngle);
	ADD_PROPERTY(m_angleVelocity);
	ADD_PROPERTY(m_initialHeight);
	ADD_PROPERTY(m_fallVelocity);
	ADD_PROPERTY(m_offset);
}

// 生成直後に一度呼ばれます
void StartCamera::Awake()
{
	// トランスフォームポインタを取得
	m_pTransform = GetComponent<Transform>();
}

// 最初のUpdate関数の直線に一度呼ばれます
void StartCamera::Start()
{
	// シーンポインタを取得
	Scene* pScene = static_cast<GameObject*>(GetOwn())->GetScene();

	// ターゲットを取得
	GameObject* target = pScene->GetObjectFinder()->FindWithNameInWorld(m_targetName);

	// 取得できなければ何もしない
	if (!target) return;

	// 保存
	m_target = target->GetComponent<Transform>()->GetWorldPosition() + m_offset;

	//----- 初期位置をセットする -----//

	// 初期高さと速度から落下し終える時間を計算
	float fallTime = m_initialHeight / m_fallVelocity;

	// 終了角度と各速度から初期角度を算出
	float initAngle = m_finalAngle - fallTime * m_angleVelocity;

	// 初期位置を算出する
	DirectX::SimpleMath::Vector3 initPos =
	{
		m_target.x + std::cosf(initAngle) * m_radius,
		m_target.y + m_initialHeight,
		m_target.z + std::sinf(initAngle) * m_radius
	};

	// セット
	m_pTransform->SetWorldPosition(initPos);				// 位置
	m_pTransform->SetLocalEulerAngle({ 0, initAngle, 0 });	// 角度

	m_nowAngle = initAngle;
}

// 毎フレーム呼ばれます
void StartCamera::Update(const GameTimer & gameTimer)
{
	// 経過時間を取得
	float elapsedTime = gameTimer.GetElapsedTime();

	// 次フレームの位置を計算する5
	const DirectX::SimpleMath::Vector3 nowPosition = m_pTransform->GetWorldPosition();
	DirectX::SimpleMath::Vector3 nextPosition;

	// 高さ
	nextPosition.y = nowPosition.y - m_fallVelocity * elapsedTime;

	// 次の角度を算出する
	m_nowAngle += m_angleVelocity * gameTimer.GetElapsedTime();

	// 位置を算出する
	nextPosition.x = m_target.x + std::sinf(m_nowAngle) * m_radius;
	nextPosition.z = m_target.z + std::cosf(m_nowAngle) * m_radius;

	// セット
	m_pTransform->SetWorldPosition(nextPosition);			// 位置
	m_pTransform->SetLocalEulerAngle({ 0, m_nowAngle, 0 });	// 角度

	// ターゲットの位置まで下りたら
	if (nextPosition.y < m_target.y)
	{
		SetActive(false);

		// TPSカメラを持っていたらアクティブにする
		if (auto* c = GetComponent<TPSCamera>())
		{
			c->SetActive(true);
		}

		// 開始イベントを呼ぶ
		EventBus<GamePlayEvent>::Publish(GamePlayEvent::Start);
	}
}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void StartCamera::LateUpdate(const GameTimer & gameTimer)
{}
