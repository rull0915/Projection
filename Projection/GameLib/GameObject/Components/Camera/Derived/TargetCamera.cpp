//====================================================//
// ファイル名  : TargetCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/20
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TargetCamera.h"

#include "GameLib/Input/KeyInput.h"

#include "../../Transform/Transform.h"

//====================================================//
// 関数の実体宣言
//====================================================//


TargetCamera::TargetCamera(IComponentOwner* own)
	: Camera(own)
	, m_nearZ{ 0.1f }
	, m_farZ{ 1000.0f }
	, m_aspect{ static_cast<float>(Screen::WIDTH) / Screen::HEIGHT }
	, m_fov{ PI_F / 4 }
	, m_target{ nullptr }
{
	UpdateView();
	UpdateProj();
}

/// <summary>
/// ビュー行列更新関数
/// </summary>
void TargetCamera::UpdateView()
{
	if (!m_target) return;

	// ---- Upベクトルを算出
	using namespace DirectX;

	// 位置を取得
	auto trans = GetOwn()->GetComponent<Transform>();
	trans->UpdateCache();
		
	SimpleMath::Vector3 cameraPos = trans->GetWorldPosition();

	auto targetTrans = m_target->GetComponent<Transform>();
	targetTrans->UpdateCache();

	SimpleMath::Vector3 targetPoint = targetTrans->GetWorldPosition();

	// 各方向ベクトル
	SimpleMath::Vector3 forward = targetPoint - cameraPos;
	forward.Normalize();

	SimpleMath::Vector3 right = SimpleMath::Vector3::UnitY.Cross(forward);
	right.Normalize();

	SimpleMath::Vector3 up = forward.Cross(right);
	up.Normalize();

	// 行列の作成
	SetView(SimpleMath::Matrix::CreateLookAt(
		cameraPos,
		targetPoint,
		up)
	);

	UpdateProj();
}

/// <summary>
/// 射影行列更新関数
/// </summary>
void TargetCamera::UpdateProj()
{
	// 各設定から射影行列を作成
	SetProj(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		m_fov,
		m_aspect,
		m_nearZ,
		m_farZ
	));

	if (KeyInput::GetKey(KeyCode::Q))
	{
		// 各種設定から行列を作成
		SetProj(DirectX::SimpleMath::Matrix::CreateOrthographic(
			m_aspect * 7.5f,
			7.5f,
			m_nearZ,
			m_farZ
		));
	}
}
