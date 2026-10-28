/**
 * @file    TPSCamera.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/07
 */

#include "pch.h"
#include "TPSCamera.h"

#include "Input/InputSystem.h"
#include <algorithm>

void TPSCamera::Awake()
{
	// トランスフォームを取得
	m_pOwnTransform = GetComponent<RHEngine::Transform>();
}

void TPSCamera::OnEnable()
{
	// マウスを相対モードに
	RHEngine::Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
}

void TPSCamera::OnDisable()
{
	// マウスを絶対モードに
	RHEngine::Input::Mouse::SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}

void TPSCamera::Update(const RHEngine::GameTimer& gameTimer)
{
	gameTimer;

	// ターゲットが設定されていなければスキップ
	if (!m_pTargetTransform) return;

	// マウスの移動量を取得
	DirectX::SimpleMath::Vector2 moveVal = { RHEngine::Input::Custom::GetAxis("CameraX"), RHEngine::Input::Custom::GetAxis("CameraY") };

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
	m_position = pos;
}
