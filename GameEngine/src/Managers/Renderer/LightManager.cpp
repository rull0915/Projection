//====================================================//
// ファイル名  : LightManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "LightManager.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void REngine::LightManager::RebuildLightData()
{
	// 予約の反映
	ReflectReserves();

	// 管理中のライトを取得
	auto& lights = GetObjects();

	// リセット
	m_lightData.pointLights.clear();
	m_lightData.spotLights.clear();
	m_lightData.directionalLights.clear();

	// 走査
	for (auto& light : lights)
	{
		// タイプを取得
		LightType type = light->GetLightType();

		// トランスフォームを取得
		Transform* transform = light->GetComponent<Transform>();

		// タイプで分岐
		switch (type)
		{
		case REngine::LightType::Point:
		{
			// PointLightを作成
			PointLight pointLight{};

			pointLight.pos = transform->GetWorldPosition();
			pointLight.radius = light->GetRadius();
			pointLight.color = light->GetColor().ToVector3();
			pointLight.intensity = light->GetIntensity();
			m_lightData.pointLights.push_back(pointLight);
		}
			break;
		case REngine::LightType::Spot:
		{
			// SpotLightを作成
			SpotLight spotLight{};

			spotLight.pos = transform->GetWorldPosition();
			spotLight.direction = transform->GetForward();
			spotLight.length = light->GetLength();
			spotLight.cosInnerAngle = std::cos(light->GetInnerAngle());
			spotLight.cosOuterAngle =  std::cos(light->GetOuterAngle());
			spotLight.color = light->GetColor().ToVector3();
			spotLight.intensity = light->GetIntensity();
			m_lightData.spotLights.push_back(spotLight);
		}
			break;
		case REngine::LightType::Directional:
		{
			// DirectionalLightを作成
			DirectionalLight directionalLight{};

			directionalLight.direction = transform->GetForward();
			directionalLight.color = light->GetColor().ToVector3();
			directionalLight.intensity = light->GetIntensity();
			m_lightData.directionalLights.push_back(directionalLight);
		}
			break;
		default:
			break;
		}
	}
}
