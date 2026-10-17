//====================================================//
// ファイル名  : ProjectionSmoothCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要       : プロジェクション行列を滑らかに切り替えるカメラ
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ProjectionSmoothCamera.h"

#include "Math/Easing.h"
#include "Components/World/Transform/Transform.h"

#include "Settings/TimeSettings.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

ProjectionSmoothCamera::ProjectionSmoothCamera(RHEngine::IComponentOwner* owner)
	: CameraBase(owner)
	, m_type{ RHEngine::ProjectionType::Perspective }
	, m_farZ{ 1000.0f }, m_nearZ{ 0.1f }
	, m_aspect{ RHEngine::WindowManager::Instance().GetAspect() }
	, m_fov{ PI_F / 4 }
	, m_size{ 15.0f }
	, m_changeTime{ 1.0f }
	, m_nowTime{ 0.0f }
	, m_isChanging{ false }
{
}

void ProjectionSmoothCamera::Update(const RHEngine::GameTimer& gameTimer)
{
	// 変化中なら
	if (m_isChanging)
	{
		// 加算する
		m_nowTime += gameTimer.GetUnScaledElapsedTime();

		SetNeedUpdateProj(true);

		// 1を超えたら
		if (m_nowTime >= m_changeTime)
		{
			m_nowTime = m_changeTime;

			// 変化を終了
			m_isChanging = false;

			// タイムスケールをもどす
			RHEngine::TimeSettings::Instance().SetTimeScale(1.0f);
		}
	}
}

void ProjectionSmoothCamera::ChangeProjectionMode(float changeTime)
{
	// 変更中は受け付けない
	if (m_isChanging) return;

	// 変化前の行列を保存
	m_oldProjecition = GetProj();

	// 変化後のタイプに変える
	m_type = (m_type == RHEngine::ProjectionType::Perspective ? RHEngine::ProjectionType::Orthographic : RHEngine::ProjectionType::Perspective);

	// 変化後の行列を作成する
	switch (m_type)
	{
		// 変化後がPerspectiveの時
	case RHEngine::ProjectionType::Perspective:

		// Perspectiveから作成
		m_targetProjection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			m_fov, m_aspect, m_nearZ, m_farZ
		);

		break;
		// 変化後がOrthographicの時
	case RHEngine::ProjectionType::Orthographic:

		// Orthographicから作成
		m_targetProjection = DirectX::SimpleMath::Matrix::CreateOrthographic(
			m_size * m_aspect, m_size, -1000.0f, m_farZ
		);

		break;
	default:
		break;
	}

	// 変化状態の初期化
	m_isChanging = true;
	m_changeTime = changeTime;
	m_nowTime = 0.0f;

	// 変化中に物理挙動などを起こさないためにタイムスケールを0にする
	RHEngine::TimeSettings::Instance().SetTimeScale(0.0f);
}

void ProjectionSmoothCamera::UpdateView()
{
	// TransformをそのままViewに
	SetView(
		GetComponent<RHEngine::Transform>()->GetWorldMatrix().Invert()
	);
}

void ProjectionSmoothCamera::UpdateProj()
{
	// 変化中なら
	if (m_isChanging)
	{
		// 変化率を算出
		float r = m_nowTime / m_changeTime;
		r = std::clamp(r, 0.0f, 1.0f);	// 0 ~ 1 にクランプ

		DirectX::SimpleMath::Matrix nowProjection{};

		// 現在の行列を求める
		switch (m_type)
		{
		case RHEngine::ProjectionType::Perspective:
			nowProjection = DirectX::SimpleMath::Matrix::Lerp(m_oldProjecition, m_targetProjection, RHEngine::Easing::EaseInCubic(r));
			break;

		case RHEngine::ProjectionType::Orthographic:
			nowProjection = DirectX::SimpleMath::Matrix::Lerp(m_oldProjecition, m_targetProjection, RHEngine::Easing::EaseOutCubic(r));
			break;

		default:
			break;
		}

		// 設定する
		SetProj(nowProjection);
	}
	// 変化中でなければ
	else
	{
		// ターゲットとなる行列を作成する
		switch (m_type)
		{
		case RHEngine::ProjectionType::Perspective:

			// Perspectiveから作成
			SetProj(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				m_fov, m_aspect, m_nearZ, m_farZ
			));

			break;
		case RHEngine::ProjectionType::Orthographic:

			// Orthographicから作成
			SetProj(DirectX::SimpleMath::Matrix::CreateOrthographic(
				m_size * m_aspect, m_size, -1000.0f, m_farZ	// カメラより手前の物も映るようにnearを負の値にします
			));

			break;
		default:
			break;
		}
	}
}
