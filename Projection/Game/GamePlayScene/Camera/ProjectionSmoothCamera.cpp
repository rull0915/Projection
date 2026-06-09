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

#include "GameLib/GameMath/GameMath.h"
#include "GameLib/GameMath/Easing.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

#include "GameLib/GameObject/Settings/TimeSettings.h"

//====================================================//
// 関数の実体宣言
//====================================================//

ProjectionSmoothCamera::ProjectionSmoothCamera(IComponentOwner* owner)
	: Camera(owner)
	, m_type{ ProjectionType::Perspective }
	, m_farZ{ 1000.0f }, m_nearZ{ 0.1f }
	, m_aspect{ static_cast<float>(Screen::WIDTH) / Screen::HEIGHT }
	, m_fov{ PI_F / 4 }
	, m_size{ 20.0f }
	, m_changeTime{ 1.0f }
	, m_nowTime{ 0.0f }
	, m_isChanging{ false }
{
}

void ProjectionSmoothCamera::Update(const GameTimer& gameTimer)
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
			TimeSettings::Instance().SetTimeScale(1.0f);
		}
	}
}

void ProjectionSmoothCamera::ChangeProjectionMode(float changeTime)
{
	// 変更中は受け付けない
	if (m_isChanging) return;

	// 今の行列を保存
	m_oldProjecition = GetProj();

	// タイプを変える
	m_type = (m_type == ProjectionType::Perspective ? ProjectionType::Orthographic : ProjectionType::Perspective);

	// ターゲットとなる行列を作成する
	switch (m_type)
	{
	case ProjectionType::Perspective:

		// Perspectiveから作成
		m_targetProjection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			m_fov, m_aspect, m_nearZ, m_farZ
		);

		break;
	case ProjectionType::Orthographic:

		// Orthographicから作成
		m_targetProjection = DirectX::SimpleMath::Matrix::CreateOrthographic(
			m_size * m_aspect, m_size, m_nearZ, m_farZ
		);

		break;
	default:
		break;
	}

	// 変化状態の初期化
	m_isChanging = true;

	m_changeTime = changeTime;
	m_nowTime = 0.0f;

	// タイムスケールを0にする
	TimeSettings::Instance().SetTimeScale(0.0f);
}

void ProjectionSmoothCamera::UpdateView()
{
	// TransformをそのままViewに
	SetView(
		GetComponent<Transform>()->GetWorldMatrix().Invert()
	);
}

void ProjectionSmoothCamera::UpdateProj()
{
	if (m_isChanging)
	{
		float r = m_nowTime / m_changeTime;
		r = MyMath::Clamp(r, 0, 1);

		DirectX::SimpleMath::Matrix nowProjection;

		// 現在の行列を求める
		switch (m_type)
		{
		case ProjectionType::Perspective:
			nowProjection = DirectX::SimpleMath::Matrix::Lerp(m_oldProjecition, m_targetProjection, Easing::EaseInCubic(r));
			break;

		case ProjectionType::Orthographic:
			nowProjection = DirectX::SimpleMath::Matrix::Lerp(m_oldProjecition, m_targetProjection, Easing::EaseOutCubic(r));
			break;

		default:
			break;
		}

		// 設定する
		SetProj(nowProjection);
	}
	else
	{
		// ターゲットとなる行列を作成する
		switch (m_type)
		{
		case ProjectionType::Perspective:

			// Perspectiveから作成
			SetProj(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				m_fov, m_aspect, m_nearZ, m_farZ
			));

			break;
		case ProjectionType::Orthographic:

			// Orthographicから作成
			SetProj(DirectX::SimpleMath::Matrix::CreateOrthographic(
				m_size * m_aspect, m_size, -1000.0f, m_farZ
			));

			break;
		default:
			break;
		}
	}
}
