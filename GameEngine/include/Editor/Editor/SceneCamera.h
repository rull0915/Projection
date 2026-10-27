/**
 * @file    SceneCamera.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   シーンビュー専用カメラ
 */

#pragma once

#ifdef ENGINE_GUI

#include <SimpleMath.h>

#include "Scene/SceneInfo.h"

namespace RHEngine
{
	/**
	 * @brief シーンビュー専用カメラ
	 */
	class SceneCamera
	{
	private:

		/** @brief 動けるかどうか */
		bool m_movable;

		/** @brief ニアクリップ */
		float m_nearZ;

		/** @brief ファークリップ */
		float m_farZ;

		/** @brief 画角 */
		float m_fov;    // 画角

		/** @brief 注視点の位置 */
		DirectX::SimpleMath::Vector3 m_targetPoint;

		/** @brief 自分の位置 */
		DirectX::SimpleMath::Vector3 m_position;

		/** @brief 自分の角度 */
		DirectX::SimpleMath::Quaternion m_rotation;

		/** @brief ビュー行列 */
		DirectX::SimpleMath::Matrix m_view;

		/** @brief 射影行列 */
		DirectX::SimpleMath::Matrix m_proj;

		/** @brief 注視点からの半径 */
		float m_radius;

		/** @brief 回転角 ヨー */
		float m_yaw = 0;

		/** @brief 回転角 ピッチ */
		float m_pitch = 0;

		/** @brief マウス移動量に対するカメラ移動の割合 */
		float m_moveRatio;

		/** @brief マウス移動量に対するカメラ回転の割合 */
		float m_rotateRatio;

		/** @brief ホイール移動量に対する拡縮の割合 */
		float m_scaleRatio;

	public:

		/// @name ライフサイクル
		/// @{

		SceneCamera();

		~SceneCamera() = default;

		/// @}


		/**
		 * @brief 初期状態の行列を計算する
		 */
		void Initialize();

		/**
		 * @brief カメラの更新をする
		 * @details キー・マウスの入力を取得し移動や回転を行う
		 */
		void Update();

		/**
		 * @brief ビュー行列を更新する
		 */
		void UpdateView();

		/**
		 * @brief 射影行列を更新する
		 */
		void UpdateProj();

		/// @name ゲッター
		/// @{

		/** @brief ビュー行列 */
		const DirectX::SimpleMath::Matrix GetView() const { return m_view; }
		
		/** @brief 射影行列 */
		const DirectX::SimpleMath::Matrix GetProj() const { return m_proj; }

		/** @brief カメラデータ */
		const SceneCameraData GetData() const;

		/// @}

		/// @name セッター
		/// @{

		/** @brief 移動可能かどうか */
		void SetMovable(bool f) { m_movable = f; }

		/// @}
	};
} // namespace RHEngine

#endif // ENGINE_GUI
