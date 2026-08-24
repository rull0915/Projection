//====================================================//
// ファイル名  : SceneCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include <SimpleMath.h>

#include "Editor/Editor/SceneCamera.h"
#include "Components/World/Transform/Transform.h"

#include "System/WindowManager.h"

#include "Input/MouseInput.h"
#include "Input/KeyInput.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	SceneCamera::SceneCamera(IComponentOwner* own)
		: CameraBase(own)
		, m_movable{ false }
		, m_nearZ{ 0.1f }
		, m_farZ{ 1000.0f }
		, m_aspect{ static_cast<float>(WindowManager::Instance().GetWidth()) / WindowManager::Instance().GetHeight() }
		, m_fov{ DirectX::XM_PI / 4 }
		, m_targetPoint{ 0, 0, 0 }
		, m_rot{ DirectX::SimpleMath::Quaternion::Identity }
		, m_radius{ 3.0f }
		, m_yaw{ 0.0f }
		, m_pitch{ 0.0f }
		, m_moveRatio{ 0.05f }
		, m_rotateRatio{ 0.01f }
		, m_scaleRatio{ 0.01f }
	{}

	// 生成直後に一度呼ばれます
	void SceneCamera::Awake()
	{}

	// 最初のUpdate関数の直線に一度呼ばれます
	void SceneCamera::Start()
	{
		// 行列を更新しておく
		DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_rot);

		// Transformを取得
		if (auto* t = GetComponent<Transform>())
		{
			// 向きを変更
			t->SetLocalRotation(
				m_rot
			);

			// 位置を変更
			t->SetLocalPosition(
				{ m_targetPoint - m_radius * forward }
			);
		}

		UpdateView();
		UpdateProj();
	}

	// 毎フレーム呼ばれます
	void SceneCamera::Update(const GameTimer& gameTimer)
	{}

	// 毎フレームUpdate及び物理挙動の後に呼ばれます
	void SceneCamera::LateUpdate(const GameTimer& gameTimer)
	{
		// 移動不可なら何もしない
		if (!m_movable) return;

		// 移動量を取得
		DirectX::SimpleMath::Vector2 moveValue = Input::Mouse::GetMouseMoveValue();

		// LeftAltが押されているとき
		if (Input::Key::Get(Input::Key::Code::LeftAlt))
		{
			// 左クリックされていたら
			if (Input::Mouse::Get(Input::Mouse::Button::Left))
			{
				// X軸回転
				m_yaw -= moveValue.x * m_rotateRatio;
				m_pitch -= moveValue.y * m_rotateRatio;

				m_pitch = std::clamp(m_pitch, -DirectX::XM_PIDIV2 + 0.01f, DirectX::XM_PIDIV2 - 0.01f);

				m_rot =
					DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_pitch) *
					DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, m_yaw);
			}
		}

		// 前方向を取得
		DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_rot);

		// 右方向
		DirectX::SimpleMath::Vector3 right;

		float rat = DirectX::SimpleMath::Vector3::Up.Dot(forward);
		if (rat < 0.8f)	right = DirectX::SimpleMath::Vector3::Up.Cross(forward);
		else right = -DirectX::SimpleMath::Vector3::Forward.Cross(forward);

		// 上方向
		DirectX::SimpleMath::Vector3 up = right.Cross(forward);

		// ホイールがクリックされていたら
		if (Input::Mouse::Get(Input::Mouse::Button::Middle))
		{
			// マウスの移動量分動かす
			m_targetPoint += (right * moveValue.x + up * -moveValue.y) * m_moveRatio;
		}

		// ホイールの移動量
		int wheel = Input::Mouse::GetWheelMoveValue();

		m_radius -= wheel * m_scaleRatio;

		m_radius = std::clamp(m_radius, 0.1f, 100.0f);

		// Transformを取得
		if (auto* t = GetComponent<Transform>())
		{
			// 向きを変更
			t->SetLocalRotation(
				m_rot
			);

			// 位置を変更
			t->SetLocalPosition(
				{ m_targetPoint - m_radius * forward }
			);
		}
	}

	/// <summary>
	/// ビュー行列更新関数
	/// </summary>
	void SceneCamera::UpdateView()
	{
		// 位置をそのまま行列に
		SetView(
			GetOwn()->GetComponent<Transform>()->GetWorldMatrix().Invert()
		);
	}

	/// <summary>
	/// 投射行列更新関数
	/// </summary>
	void SceneCamera::UpdateProj()
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
