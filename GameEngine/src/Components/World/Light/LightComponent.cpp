/**
 * @file    LightComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/11
 */

#include "pch.h"
#include "Components/World/Light/LightComponent.h"
#include "Renderer/Renderer.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	// コンストラクタ
	LightComponent::LightComponent(IComponentOwner* pOwner)
		: WorldComponentBase(pOwner)
		, m_lightType{ LightType::None }
		, m_color{ 1, 1, 1, 1 }
		, m_intensity{ 1.0f }
		, m_radius{ 1.0f }
		, m_length{ 1.0f }
		, m_innerAngle{ 0.0f }
		, m_outerAngle{ 1.0f }
	{
		ADD_PROPERTY(LightComponent, m_lightType);
		ADD_PROPERTY(LightComponent, m_color);
		ADD_PROPERTY(LightComponent, m_intensity);
	}

	std::vector<Property> LightComponent::GetProperties()
	{
		// プロパティ一覧を格納する配列
		std::vector<Property> properties{};

		// デフォルトのプロパティを取得
		properties = PropertyObject::GetProperties();

		// 1つのfloat変数を追加するラムダ式
		auto addOnceFloat = [&properties](const std::string& name, void* pValue)
			{
				properties.push_back(
					Property(
						name, PropertyType::Float, pValue
					)
				);
			};

		// ライトタイプで分岐
		switch (m_lightType)
		{
		case RHEngine::LightType::Spot:

			addOnceFloat("length", &m_length);
			addOnceFloat("innerAngle", &m_innerAngle);
			addOnceFloat("outerAngle", &m_outerAngle);

			break;
		case RHEngine::LightType::Point:

			// 半径を追加
			addOnceFloat("radius", &m_radius);

			break;

			// 平行光源は追加変数なし
		case RHEngine::LightType::None:

			addOnceFloat("length", &m_length);
			addOnceFloat("innerAngle", &m_innerAngle);
			addOnceFloat("outerAngle", &m_outerAngle);
			addOnceFloat("radius", &m_radius);

			break;
		default:
			break;
		}

		return properties;
	}

	LightComponent::~LightComponent()
	{}

	void LightComponent::OnValidate()
	{
		// 内角度と外角度の関係を保証する
		if (m_innerAngle > m_outerAngle)
		{
			m_innerAngle = m_outerAngle;
		}

		// 強度は0以上を保証する
		if (m_intensity < 0.0f)
		{
			m_intensity = 0.0f;
		}

		// 半径は0以上を保証する
		if (m_radius < 0.0f)
		{
			m_radius = 0.0f;
		}
	}

	void LightComponent::DebugRender(Renderer& renderer)
	{
		// タイプによって変更
		switch (m_lightType)
		{
		case RHEngine::LightType::Point:
			DebugDrawPointLight(renderer);
			break;
		case RHEngine::LightType::Spot:
			break;
		case RHEngine::LightType::Directional:
			break;
		default:
			break;
		}
	}

	void LightComponent::DebugDrawPointLight(Renderer& renderer)
	{
		// 中心座標を取得
		Transform* pTransform = GetComponent<Transform>();

		DirectX::SimpleMath::Vector3 center = pTransform->GetWorldPosition();

		renderer.Draw().Primitive().DrawCircle(
			center, DirectX::SimpleMath::Vector3::Up, m_radius, 16, m_color, false
		);
		renderer.Draw().Primitive().DrawCircle(
			center, DirectX::SimpleMath::Vector3::Right, m_radius, 16, m_color, false
		);
		renderer.Draw().Primitive().DrawCircle(
			center, DirectX::SimpleMath::Vector3::Forward, m_radius, 16, m_color, false
		);
	}
}	// namespace RHEngine
