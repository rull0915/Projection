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
	// カメラの情報
	struct SceneCameraData
	{
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		DirectX::SimpleMath::Vector3 pos;
		Handle<MaterialAsset> materialHandle;
	};

	// ライトの情報
	struct SceneLightData
	{
		std::vector<PointLight> pointLights;
		std::vector<SpotLight> spotLights;
		std::vector<DirectionalLight> directionalLights;
	};

	// 時間の情報
	struct SceneTimeData
	{
		float time;
		float elapsedTime;
	};

	// シーンの情報
	struct SceneData
	{
		SceneCameraData cameraData;
		SceneLightData lightData;
		SceneTimeData timeData;
	};
}
