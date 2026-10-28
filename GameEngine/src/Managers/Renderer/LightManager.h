/**
 * @file    LightManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/11
 *
 * @brief   ライトコンポーネントを管理するクラス
 */

#pragma once

#include "Components/World/Light/LightComponent.h"
#include "Managers/ReserveContainer.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	/**
	 * @brief ライトコンポーネントの管理
	 */
	class LightManager : public ReserveContainer<LightComponent>
	{
	private:

		/** @brief シーンにあるライトのデータ */
		SceneLightData m_lightData;

	public:
		
		/// @name ライフサイクル
		/// @{

		LightManager()
			: m_lightData{}
		{
		};

		~LightManager() = default;

		/// @}
		
		/**
		 * @brief ライトデータを再構築する
		 */
		void RebuildLightData();

		/**
		 * @brief ライトデータを取得する
		 */
		const SceneLightData& GetLightData() const { return m_lightData; }

		/**
		 * @brief カメラのデータとライトからライト視点のカメラを作成する
		 * @details ディレクショナルライトの場合：カメラの試錐台が全て映るようにライトのプロジェクションを作成する
		 * @param cameraData 描画するカメラのデータ
		 * @param light 対象のライト
		 * @param shadowMapSize シャドウマップのサイズ
		 * @return 作成したデータ
		 */
		SceneCameraData GetLightData(const SceneCameraData& cameraData, LightComponent* pLight, float shadowMapSize) const;

		/**
		 * @brief タイプ指定でライトを一つ取得する
		 * @param type 
		 * @return 
		 */
		LightComponent* GetLight(LightType type);

	private:

		/**
		 * @brief ディレクショナルライトのカメラデータを構築する
		 */
		SceneCameraData GetDirectionalLightData(const SceneCameraData& cameraData, LightComponent* light, float shadowMapSize) const;
	};
}	// namespace RHEngine
