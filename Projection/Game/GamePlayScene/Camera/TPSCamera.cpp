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

#include "GameLib/Input/MouseInput.h"
#include <algorithm>

//====================================================//
// 関数の実体宣言
//====================================================//

void TPSCamera::Awake()
{
	// トランスフォームを取得
	m_pOwnTransform = GetComponent<Transform>();

	// マウスを相対モードに
//	MouseInput::SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
}

void TPSCamera::Start()
{}

void TPSCamera::LateUpdate(const GameTimer& gameTimer)
{
	// ターゲットが設定されていなければスキップ
	if (!m_pTargetTransform) return;

	// マウスの移動量を取得
	DirectX::SimpleMath::Vector2 moveVal = MouseInput::GetMouseMoveValue();

	// 角度に加算する
	m_angle.x += moveVal.y * SENSITIVITY * 0.01f;
	m_angle.y -= moveVal.x * SENSITIVITY * 0.01f;

	// クランプする
	m_angle.x = std::clamp(m_angle.x, -PI_F * (15.0f / 32.0f), PI_F * (15.0f / 32.0f));

	// 角度に合うように位置を補正する
	DirectX::SimpleMath::Vector3 pos = m_pTargetTransform->GetLocalPosition();

	// 位置を算出
	pos += DirectX::SimpleMath::Vector3{ 
		std::cosf(m_angle.x) * std::sinf(m_angle.y), 
		std::sinf(m_angle.x), 
		std::cosf(m_angle.x) * std::cosf(m_angle.y) } 
	* DISTANCE;

	// 位置を設定
	m_pOwnTransform->SetLocalPosition(pos);

	// 角度を設定
	m_pOwnTransform->SetLocalEulerAngle({ -m_angle.x, m_angle.y, 0.0f });

	// ターゲットの角度を設置
	m_pTargetTransform->SetLocalEulerAngle({ 0, m_angle.y, 0.0f });
}
