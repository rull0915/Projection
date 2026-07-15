//====================================================//
// ファイル名  : StandardCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/20
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Camera/Derived/StandardCamera.h"

#include "Components/World/Transform/Transform.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	StandardCamera::StandardCamera(IComponentOwner* own)
		: CameraBase(own)
		, m_nearZ{ 0.1f }
		, m_farZ{ 1000.0f }
		, m_aspect{ static_cast<float>(WindowManager::Instance().GetWidth()) / WindowManager::Instance().GetHeight() }
		, m_fov{ PI_F / 4 }
	{
		ADD_PROPERTY(m_nearZ);
		ADD_PROPERTY(m_farZ);
		ADD_PROPERTY(m_aspect);
		ADD_PROPERTY(m_fov);

		UpdateView();
		UpdateProj();
	}

	/// <summary>
	/// ビュー行列更新関数
	/// </summary>
	void StandardCamera::UpdateView()
	{
		// 位置をそのまま行列に
		SetView(
			GetOwn()->GetComponent<Transform>()->GetWorldMatrix().Invert()
		);
	}

	/// <summary>
	/// 投射行列更新関数
	/// </summary>
	void StandardCamera::UpdateProj()
	{
		// 各設定から射影行列を作成
		SetProj
		(
			DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				m_fov,
				m_aspect,
				m_nearZ,
				m_farZ
			)
		);
	}
}	// namespace REngine
