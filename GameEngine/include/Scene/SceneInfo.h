//====================================================//
// ファイル名   : SceneInfo.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要 : シーンの情報に関する構造体をまとめたヘッダ
//
// 更新履歴 :
// 2026/10/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <SimpleMath.h>
#include "Common/LightData.h"
#include "Assets/Types/Material/MaterialAsset.h"

namespace REngine
{
	// ライトの情報
	struct SceneLightData
	{
		std::vector<PointLight> pointLights{};
		std::vector<SpotLight> spotLights{};
		std::vector<DirectionalLight> directionalLights{};
	};

	// 時間の情報
	struct SceneTimeData
	{
		float time = 0.0f;
		float elapsedTime = 0.0f;
	};

	// カメラの情報
	struct SceneCameraData
	{
		DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix proj = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;
	};
	
	// 影の情報
	struct SceneShadowData
	{
		SceneCameraData lightCameraData{};
		float shadowBias = 0.0f;
	};

	// シーンの情報
	struct SceneData
	{
		SceneLightData lightData{};
		SceneTimeData timeData{};
		SceneShadowData shadowData{};
	};
}
