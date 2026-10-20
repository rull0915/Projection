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
#include "Math/Bounding.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	void LightManager::RebuildLightData()
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
			if (!light->IsActive() || !light->GetOwn()->IsActive()) continue;

			// タイプを取得
			LightType type = light->GetLightType();

			// トランスフォームを取得
			Transform* transform = light->GetComponent<Transform>();

			// タイプで分岐
			switch (type)
			{
			case LightType::Point:
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
			case LightType::Spot:
			{
				// SpotLightを作成
				SpotLight spotLight{};

				spotLight.pos = transform->GetWorldPosition();
				spotLight.direction = transform->GetForward();
				spotLight.length = light->GetLength();
				spotLight.cosInnerAngle = std::cos(light->GetInnerAngle());
				spotLight.cosOuterAngle = std::cos(light->GetOuterAngle());
				spotLight.color = light->GetColor().ToVector3();
				spotLight.intensity = light->GetIntensity();
				m_lightData.spotLights.push_back(spotLight);
			}
			break;
			case LightType::Directional:
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

	SceneCameraData LightManager::GetLightData(const SceneCameraData& cameraData, LightComponent* light, float shadowMapSize) const
	{
		if (!light) return SceneCameraData{};

		switch (light->GetLightType())
		{
		case LightType::Directional:
			return GetDirectionalLightData(cameraData, light, shadowMapSize);
		default:
			return SceneCameraData{};
		}
	}

	LightComponent* LightManager::GetLight(LightType type)
	{
		// 管理中のライトリストを取得
		auto& lights = GetObjects();

		// ライトリストから検索
		auto it = std::find_if(
			lights.begin(), lights.end(),
			[type](LightComponent* light) -> bool
			{
				// 指定されたタイプと一致するものを取得
				return light->GetLightType() == type;
			}
		);

		// 見つかれば
		if (it != lights.end())
		{
			return *it;
		}

		// 見つからなかった場合
		return nullptr;
	}

	SceneCameraData LightManager::GetDirectionalLightData(const SceneCameraData& cameraData, LightComponent* light, float shadowMapSize) const
	{
		//----- カメラのVPから描画範囲を表す点を作成する -----//

		// カメラVP逆行列を作成
		DirectX::SimpleMath::Matrix invVP =
			(cameraData.view * cameraData.proj).Invert();

		// -1 ~ 1 の8点を変換
		DirectX::SimpleMath::Vector3 points[8]{};

		// 8回ループ
		for (int i = 0; i < 8; i++)
		{
			// -1, 1を8頂点が出来るようにbit演算
			DirectX::SimpleMath::Vector3 beforeP =
			{
				i & 1 ? 1.0f : -1.0f,
				i & 2 ? 1.0f : -1.0f,
				i & 4 ? 1.0f : 0.0f
			};

			// 変換
			points[i] = DirectX::SimpleMath::Vector3::Transform(beforeP, invVP);
		}

		// 8点の中心点を求める
		DirectX::SimpleMath::Vector3 frustumCenter{};
		for (auto& p : points) frustumCenter += p;
		frustumCenter /= 8;

		//----- 描画範囲からライトVPを作成する -----//

		// ライトの向きを取得
		DirectX::SimpleMath::Vector3 lightDir = light->GetComponent<Transform>()->GetForward();

		// 仮のLightView行列を作成

		// 中心点を見るようにライトの位置を作成
		DirectX::SimpleMath::Vector3 lightPos = frustumCenter - lightDir;

		// ベクトルが近すぎないように上方向を作成
		DirectX::SimpleMath::Vector3 up = (fabs(lightDir.y) > 0.99f) ? DirectX::SimpleMath::Vector3::Forward : DirectX::SimpleMath::Vector3::Up;

		// ビュー行列を作成
		DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(lightPos, frustumCenter, up);

		// 8頂点をライト空間へ変換しAABBを求める
		BoundingBox box{ 
			{ FLT_MAX,  FLT_MAX,  FLT_MAX }, 
			{ -FLT_MAX, -FLT_MAX, -FLT_MAX } 
		};

		// 8回ループ
		for (int i = 0; i < 8; ++i)
		{
			DirectX::SimpleMath::Vector3 p = DirectX::SimpleMath::Vector3::Transform(points[i], lightView);

			box.min = DirectX::SimpleMath::Vector3::Min(p, box.min);
			box.max = DirectX::SimpleMath::Vector3::Max(p, box.max);
		}

		// ライトの手前にある物も反映させるためにnearを動かす
		float shadowCasterExtension = 100.0f;
	//	box.min.z -= shadowCasterExtension;

		//------ テクセルスナップ ------//

		// 1テクセルあたりのワールドサイズを計算
		float worldPerTexel = (box.max.x - box.min.x) / shadowMapSize;

		// テクセルサイズ単位で丸める
		box.min.x = floor(box.min.x / worldPerTexel) * worldPerTexel;
		box.max.x = ceil (box.max.x / worldPerTexel) * worldPerTexel;
		box.min.y = floor(box.min.y / worldPerTexel) * worldPerTexel;
		box.max.y = ceil (box.max.y / worldPerTexel) * worldPerTexel;

		//------ Projection行列を作成 ------//
		DirectX::SimpleMath::Matrix lightProj =
			DirectX::SimpleMath::Matrix::
			CreateOrthographicOffCenter(box.min.x, box.max.x, box.min.y, box.max.y, box.min.z, box.max.z);

		// 構築するライト視点のカメラデータ
		SceneCameraData lightCameraData{};

		lightCameraData.view = lightView;
		lightCameraData.proj = lightProj;
		lightCameraData.pos = lightPos;

		return lightCameraData;
	}
}
