//====================================================//
// ファイル名  : GraphicsSystem.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要        :  
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/GraphicsSystem.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================// }

	FrameInfo REngine::GraphicsSystem::CreateFrameInfo(const SceneData& sceneData)
	{
		// フレーム情報の作成
		FrameInfo frameInfo{};

		// カメラ情報の設定
		CameraInfo cameraInfo{};

		// VP行列を取得
		DirectX::SimpleMath::Matrix view = sceneData.cameraData.view;
		DirectX::SimpleMath::Matrix proj = sceneData.cameraData.proj;
		DirectX::SimpleMath::Matrix vp = view * proj;

		// GPU用に転置して作成
		cameraInfo.view = view.Transpose();
		cameraInfo.proj = proj.Transpose();
		cameraInfo.vpMat = vp.Transpose();               
		cameraInfo.invVpMat = vp.Invert().Transpose();    
		frameInfo.camera = cameraInfo;

		// ライト情報の設定
		LightInfo lightInfo{};

		lightInfo.pointLightCount = std::min(MAX_POINT_LIGHT, static_cast<int>(sceneData.lightData.pointLights.size()));
		lightInfo.spotLightCount = std::min(MAX_SPOT_LIGHT, static_cast<int>(sceneData.lightData.spotLights.size()));
		lightInfo.directionalLightCount = std::min(MAX_DIRECTIONAL_LIGHT, static_cast<int>(sceneData.lightData.directionalLights.size()));

		for (int i = 0; i < lightInfo.pointLightCount; ++i)
		{
			// ポイントライト情報の設定
			lightInfo.pointLights[i] = sceneData.lightData.pointLights[i];
		}

		for (int i = 0; i < lightInfo.spotLightCount; ++i)
		{
			// スポットライト情報の設定
			lightInfo.spotLights[i] = sceneData.lightData.spotLights[i];
		}

		for (int i = 0; i < lightInfo.directionalLightCount; ++i)
		{
			// ディレクショナルライト情報の設定
			lightInfo.directionalLights[i] = sceneData.lightData.directionalLights[i];
		}

		frameInfo.light = lightInfo;

		// 時間情報の設定
		TimeInfo timeInfo{};

		timeInfo.time = sceneData.timeData.time;
		timeInfo.elapsedTime = sceneData.timeData.elapsedTime;

		frameInfo.time = timeInfo;

		return frameInfo;
	}
}
