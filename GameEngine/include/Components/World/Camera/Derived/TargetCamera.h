/**
 * @file    TargetCamera.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/20
 *
 * @brief   注視点を扱うカメラ
 */

#pragma once

#include "../CameraBase.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief 注視点を扱うカメラコンポーネント
	 */
	class TargetCamera : public CameraBase
	{
	public:
		DECLARE_COMPONENT_TYPE(CameraBase);

	private:

		/** @brief ニアクリップ */
		float m_nearZ;

		/** @brief ファークリップ */
		float m_farZ;

		/** @brief 画角 */
		float m_fov;

		/** @brief 見る対象 */
		IComponentOwner* m_pTarget;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		TargetCamera(IComponentOwner* own);

		~TargetCamera() {};

		/// @}

		/**
		 * @brief ビュー行列を更新する
		 */
		void UpdateView() override;

		/**
		 * @brief 射影行列を更新する
		 */
		void UpdateProj() override;

		/// @name ゲッター
		/// @{

		/** @brief 画角 */
		float GetFov() const { return m_fov; }

		/** @brief ニアクリップ */
		float GetNearClip() const { return m_nearZ; }

		/** @brief ファークリップ */
		float GetFarClip() const { return m_farZ; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 画角 */
		void SetFov(float f)
		{
			m_fov = f;
			SetNeedUpdateProj(true);
		}

		/** @brief ニアクリップ */
		void SetNearClip(float n)
		{
			m_nearZ = n;
			SetNeedUpdateProj(true);
		}

		/** @brief ファークリップ */
		void SetFarClip(float f)
		{
			m_farZ = f;
			SetNeedUpdateProj(true);
		}

		/** @brief 注視対象 */
		void SetTarget(IComponentOwner* target) 
		{
			m_pTarget = target; 
		}

		/// @}
	};
} // namespace RHEngine
