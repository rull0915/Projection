/**
 * @file    CameraManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 *
 * @brief   カメラ管理クラス
 */

#pragma once

#include "ReserveContainer.h"
#include "Components/World/Camera/CameraBase.h"

namespace RHEngine
{
	class Scene;

	/**
	 * @brief カメラコンポーネントを管理する
	 */
	class CameraManager : public ReserveContainer<CameraBase>
	{
	public:

		/// @name ライフサイクル
		/// @{

		CameraManager();

		~CameraManager();

		/// @}

		/**
		 * @brief メインカメラを取得する
		 */
		CameraBase* GetMainCamera();
	};
}	// namespace RHEngine
