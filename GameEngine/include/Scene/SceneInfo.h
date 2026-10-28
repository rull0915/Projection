/**
 * @file    SceneInfo.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/11
 *
 * @brief   シーンの情報に関する構造体をまとめたヘッダ
 */

#include <vector>
#include <SimpleMath.h>

#include "Common/LightData.h"

namespace RHEngine
{
	/**
	 * @brief シーンにあるライトのデータ
	 */
	struct SceneLightData
	{
		std::vector<PointLight> pointLights{};	///< ポイントライトの情報
		std::vector<SpotLight> spotLights{};	///< スポットライトの情報
		std::vector<DirectionalLight> directionalLights{};	///< ディレクショナルライトの情報
	};

	/**
	 * @brief 時間の情報
	 */
	struct SceneTimeData
	{
		float time = 0.0f;			///< プロジェクト開始からの総経過時間
		float elapsedTime = 0.0f;	///< 1fの経過時間
	};

	/**
	 * @brief シーンカメラデータ
	 */
	struct SceneCameraData
	{
		DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;	///< ビュー行列
		DirectX::SimpleMath::Matrix proj = DirectX::SimpleMath::Matrix::Identity;	///< 射影行列	
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;		///< カメラの位置
	};
	
	/**
	 * @brief 影のデータ
	 */
	struct SceneShadowData
	{
		SceneCameraData lightCameraData{};	///< ライトをカメラとしたときのカメラデータ
		float shadowBias = 0.0f;	///< シャドウバイアス
	};

	/**
	 * @brief シーン全体のデータ
	 */
	struct SceneData
	{
		SceneLightData lightData{};		///< ライト
		SceneTimeData timeData{};		///< 時間
		SceneShadowData shadowData{};	///< 影
	};
}	// namespace RHEngine
