//====================================================//
// ファイル名  : LightComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要       : LightComponentシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Light/LightComponent.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	LightComponent::LightComponent(IComponentOwner* owner)
		: WorldComponentBase(owner)
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
		auto addOnceFloat = [&properties](const std::string& name, void* value)
			{
				properties.push_back(
					Property(
						name, PropertyType::Float, value
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
}
