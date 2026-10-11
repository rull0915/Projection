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

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	LightComponent::LightComponent(IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_lightType{ LightType::Directional }
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

		// ライトタイプで分岐
		switch (m_lightType)
		{
		case REngine::LightType::Spot:

			// 距離を追加
			properties.push_back(
				Property(
					"length",				// プロパティ名の指定
					PropertyType::Float,	// プロパティタイプの指定
					&m_length)				// プロパティ本体の指定
			);

			// 内角度を追加
			properties.push_back(
				Property(
					"innerAngle",				// プロパティ名の指定
					PropertyType::Float,	// プロパティタイプの指定
					&m_innerAngle)				// プロパティ本体の指定
			);

			// 外角度を追加
			properties.push_back(
				Property(
					"outerAngle",				// プロパティ名の指定
					PropertyType::Float,	// プロパティタイプの指定
					&m_outerAngle)				// プロパティ本体の指定
			);

			// 半径はスポットライト、ポイントライト共通のため意図的なフォールスルー
			[[fallthrough]];
		case REngine::LightType::Point:

			// 半径を追加
			properties.push_back(
				Property(
					"radius",				// プロパティ名の指定
					PropertyType::Float,	// プロパティタイプの指定
					&m_radius)				// プロパティ本体の指定
			);

			break;
			// 平行光源は追加変数なし
		case REngine::LightType::Directional:
			break;
		default:
			break;
		}

		return properties;
	}

	LightComponent::~LightComponent()
	{}
}
