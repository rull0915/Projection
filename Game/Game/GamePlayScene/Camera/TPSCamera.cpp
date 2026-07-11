//====================================================//
// ファイル名  : TPSCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/07
//
// 概要       : カメラをTPS用に動かすコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TPSCamera.h"

#include "Input/MouseInput.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include <algorithm>

//====================================================//
// 関数の実体宣言
//====================================================//

void TPSCamera::Awake()
{
	// トランスフォームを取得
	m_pOwnTransform = GetComponent<Transform>();

	// マウスを相対モードに
	Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
}

void TPSCamera::Start()
{
	// 既に設定されていれば
	if (!m_targetName.empty())
	{
		// 読み込み
		SetTarget(m_targetName);
	}
}

void TPSCamera::LateUpdate(const GameTimer& gameTimer)
{
	gameTimer;

	// ターゲットが設定されていなければスキップ
	if (!m_pTargetTransform) return;

	// マウスの移動量を取得
	DirectX::SimpleMath::Vector2 moveVal = Input::Mouse::GetMouseMoveValue();

	// 角度に加算する
	m_angle.x += moveVal.y * m_sensitivity * 0.01f;
	m_angle.y -= moveVal.x * m_sensitivity * 0.01f;

	// クランプする
	m_angle.x = std::clamp(m_angle.x, -PI_F * (10.0f / 32.0f), PI_F * (10.0f / 32.0f));

	// 角度に合うように位置を補正する
	DirectX::SimpleMath::Vector3 pos = m_pTargetTransform->GetWorldPosition();

	// 位置を算出
	pos += DirectX::SimpleMath::Vector3{ 
		std::cosf(m_angle.x) * std::sinf(m_angle.y), 
		std::sinf(m_angle.x), 
		std::cosf(m_angle.x) * std::cosf(m_angle.y) } 
		* m_distance;

	// 位置を設定
	m_pOwnTransform->SetLocalPosition(pos);

	// 角度を設定
	m_pOwnTransform->SetLocalEulerAngle({ -m_angle.x, m_angle.y, 0.0f });

	// ターゲットの角度を設置
	m_pTargetTransform->SetLocalEulerAngle({ 0, m_angle.y, 0.0f });
}

void TPSCamera::SetTarget(const std::string& name)
{
	// ターゲットにセット
	m_targetName = name;

	// 名前でオブジェクトを検索する
	GameObject* obj = static_cast<GameObject*>(GetOwn())->GetScene()->GetObjectFinder()->FindWithNameInWorld("Player");

	// 見つかれば
	if (obj)
	{
		m_pTargetTransform = obj->GetComponent<Transform>();
	}
}

void TPSCamera::OnDestroy()
{
	// 削除時はマウスを絶対モードに
	Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}
