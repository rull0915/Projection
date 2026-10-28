/**
 * @file    TargetCamera.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/20
 */

#include "pch.h"
#include "Components/World/Camera/Derived/TargetCamera.h"

#include "Components/World/Transform/Transform.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	TargetCamera::TargetCamera(IComponentOwner* own)
		: CameraBase(own)
		, m_nearZ{ 0.1f }
		, m_farZ{ 1000.0f }
		, m_fov{ PI_F / 4 }
		, m_pTarget{ nullptr }
	{
		ADD_PROPERTY(TargetCamera, m_nearZ);
		ADD_PROPERTY(TargetCamera, m_farZ);
		ADD_PROPERTY(TargetCamera, m_fov);

		UpdateView();
		UpdateProj();
	}

	/// <summary>
	/// ビュー行列更新関数
	/// </summary>
	void TargetCamera::UpdateView()
	{
		if (!m_pTarget) return;

		// ---- Upベクトルを算出
		using namespace DirectX;

		// 位置を取得
		auto pTransform = GetOwn()->GetComponent<Transform>();
		pTransform->UpdateCache();

		SimpleMath::Vector3 cameraPos = pTransform->GetWorldPosition();

		auto pTargetTransform = m_pTarget->GetComponent<Transform>();
		pTargetTransform->UpdateCache();

		SimpleMath::Vector3 targetPoint = pTargetTransform->GetWorldPosition();

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
		// アスペクト比を取得
		float aspect = WindowManager::Instance().GetAspect();

		// 各設定から射影行列を作成
		SetProj(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			m_fov,
			aspect,
			m_nearZ,
			m_farZ
		));
	}
}	// namespace RHEngine
