/**
 * @file    StandardCamera.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/20
 */

#include "pch.h"
#include "Components/World/Camera/Derived/StandardCamera.h"

#include "Components/World/Transform/Transform.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	StandardCamera::StandardCamera(IComponentOwner* pOwner)
		: CameraBase(pOwner)
		, m_nearZ{ 0.1f }
		, m_farZ{ 1000.0f }
		, m_fov{ PI_F / 4 }
	{
		ADD_PROPERTY(StandardCamera, m_nearZ);
		ADD_PROPERTY(StandardCamera, m_farZ);
		ADD_PROPERTY(StandardCamera, m_fov);

		UpdateView();
		UpdateProj();
	}

	void StandardCamera::UpdateView()
	{
		// 位置をそのまま行列に
		SetView(
			GetOwn()->GetComponent<Transform>()->GetWorldMatrix().Invert()
		);
	}

	void StandardCamera::UpdateProj()
	{
		// アスペクト比を取得
		float aspect = WindowManager::Instance().GetAspect();

		// 各設定から射影行列を作成
		SetProj
		(
			DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				m_fov,
				aspect,
				m_nearZ,
				m_farZ
			)
		);
	}

	void StandardCamera::OnValidate()
	{
		if (m_nearZ < 0.001f) m_nearZ = 0.001f;
		if (m_farZ < 0.002f) m_farZ = 0.002f;
		if (m_nearZ > m_farZ - 0.001f) m_nearZ = m_farZ - 0.001f;

		CameraBase::OnValidate();
	}
}	// namespace RHEngine
